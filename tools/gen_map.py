#!/usr/bin/env python3
"""Generate sk83_na_m.map for the guest's assert callstack resolver.

Format mirrors a real Xbox 360 MSVC linker map (reference:
"H:\\Skate 2 (May 06 2009)\\sk82_na_zd.map"), because EA's EACallstack picks
its parser from a marker in the first 255 bytes:

    "Timestamp is"                       -> type 1 -> MapFileMSVC   <= we want this
    "  Starting        Virtual  File"    -> type 4 -> MapFileMSVC
    "Address  Size     Align Out ..."    -> type 3 -> MapFileSN
    (no marker)                          -> type 2 -> MapFileGCC3

Layout, copied from the reference:

     <name>
    (blank)
     Timestamp is <hex> (<date>)
    (blank)
     Preferred load address is 82000000
    (blank)
     Start         Length     Name                   Class
     0001:00000000 013c9528H .text                   CODE
    (blank)
      Address         Publics by Value              Rva+Base       Lib:Object
    (blank)
     0001:004e0000       ?Foo@Bar@@QAAXXZ           824e0000 f i sk83.obj

Rows start with ONE SPACE and are variable width - the reference has 748
distinct row widths, so neither flush-left nor fixed-width padding is
required (earlier guesses at both were wrong).

Which file the game opens: NOT the "GAME:\\sk83_na_m.map" string in
PrepareCallstack. The real loader (sub_82D15AB8) calls
EA::Callstack::GetCurrentProcessPath (0x82D09B40) and appends ".map", so the
name must match the module - see --module.
"""
import argparse
import re
import sys

TOML_LINE = re.compile(r'^\s*(0x[0-9A-Fa-f]+)\s*=\s*\{(.*)\}\s*$')
NAME = re.compile(r'name\s*=\s*"([^"]*)"')
REX = re.compile(r'^rex_(.*)_([0-9A-Fa-f]{8})$')
WS = re.compile(r'\s+')


def load_toml(path):
    out = []
    with open(path, encoding='utf-8', errors='replace') as f:
        for line in f:
            m = TOML_LINE.match(line.rstrip('\n'))
            if not m:
                continue
            nm = NAME.search(m.group(2))
            out.append((int(m.group(1), 16), nm.group(1) if nm else None))
    out.sort()
    return out


def load_mangled(path):
    """addr -> real MSVC mangled symbol (preferred for display)."""
    out = {}
    try:
        f = open(path, encoding='utf-8')
    except OSError:
        return out
    with f:
        head = f.readline().rstrip('\n').split('\t')
        try:
            ia, im = head.index('sk83_addr'), head.index('mangled_symbol')
        except ValueError:
            return out
        for line in f:
            p = line.rstrip('\n').split('\t')
            if len(p) > max(ia, im):
                try:
                    out[int(p[ia], 16)] = p[im]
                except ValueError:
                    pass
    return out


def no_space(s):
    """Both parsers read the symbol with %s, which stops at whitespace."""
    return WS.sub('_', s.strip())


def display_name(addr, toml_name, mangled):
    sym = mangled.get(addr)
    if sym:
        return no_space(sym)
    if toml_name:
        if toml_name.startswith('rex_unknown_'):
            return 'sub_%08X' % addr
        m = REX.match(toml_name)
        return no_space(m.group(1) if m else toml_name)
    return 'sub_%08X' % addr


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--toml', default='sk83_na_m_config.toml')
    ap.add_argument('--mangled', default='assets/sk83_function_matches_final.tsv')
    ap.add_argument('--out', default='assets/map/sk83_na_m.map')
    ap.add_argument('--module', default='sk83_na_m',
                    help='module name written in the header; the loader opens '
                         '<module path>.map, so this should match the .xex')
    ap.add_argument('--base', type=lambda s: int(s, 0), default=0x82000000,
                    help='image base (xextool "Load Address")')
    ap.add_argument('--timestamp', default='4B1CBB3A',
                    help='xextool "Filetime" of the xex')
    ap.add_argument('--date', default='Mon Dec 07 10:22:18 2009')
    ap.add_argument('--no-stubs', action='store_true',
                    help='omit unnamed functions instead of emitting sub_<ADDR>')
    a = ap.parse_args()

    funcs = load_toml(a.toml)
    if not funcs:
        sys.exit('no functions parsed from %s' % a.toml)
    mangled = load_mangled(a.mangled)

    rows = []
    for addr, nm in funcs:
        disp = display_name(addr, nm, mangled)
        if a.no_stubs and disp.startswith('sub_'):
            continue
        rows.append((addr, disp))
    if not rows:
        sys.exit('no rows to write')

    lo, hi = rows[0][0], rows[-1][0]
    # One CODE section covering every function, section-relative to the image
    # base (matches the reference, where section:offset is section-relative
    # and the Rva+Base column is absolute).
    sec_len = (hi - a.base) + 0x10

    named = sum(1 for _, n in funcs if n)
    with open(a.out, 'w', encoding='utf-8', newline='\r\n') as f:
        f.write(' %s\n\n' % a.module)
        f.write(' Timestamp is %s (%s)\n\n' % (a.timestamp.lower(), a.date))
        f.write(' Preferred load address is %08x\n\n' % a.base)
        f.write(' Start         Length     Name                   Class\n')
        f.write(' 0001:%08x %08xH .text                   CODE\n\n'
                % (0, sec_len))
        f.write('  Address         Publics by Value              '
                'Rva+Base       Lib:Object\n\n')
        for addr, disp in rows:
            f.write(' 0001:%08x       %-26s %08x f i %s.obj\n'
                    % (addr - a.base, disp, addr, a.module))

    print('wrote %s' % a.out)
    print('  functions: %d   named: %d   sub_ stubs: %d'
          % (len(rows), named, len(rows) - named))
    print('  address range: 0x%08X - 0x%08X' % (lo, hi))
    print('  copy next to the xex as %s.map (loader derives the name from the'
          ' running module)' % a.module)


if __name__ == '__main__':
    main()
