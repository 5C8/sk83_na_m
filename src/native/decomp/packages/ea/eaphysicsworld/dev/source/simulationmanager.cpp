// EA::Physics::SimulationManager solver job entry points (EA side of the physics job).
// SimulationManager::UpdateEntryPoints (0x830ac5c8) registers these with EA::Jobs; each job's
// data block holds the Simulation* at +0x00 (and, for the island iterations, the island at
// +0x0c). Names without a map entry are ours, after the job names UpdateEntryPoints sets.
#include "simulation.h"

namespace EA { namespace Physics {

struct SolverJobData
{
    rw::physics::Simulation *simulation;   // 0x00
    unsigned int pad04[2];
    unsigned int island;                   // 0x0c
};

class SimulationManager
{
public:
    static void CreateSimulationIslandTablesJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void ContactCompilerJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void JointCompilerJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void DriveCompilerJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void IslandIterationsJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void IterationsJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void DynamicUpdateJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void ContactSpiesJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void JointSpiesJob(unsigned int data, unsigned int, unsigned int, unsigned int);
    static void DriveSpiesJob(unsigned int data, unsigned int, unsigned int, unsigned int);
};

#define JOB_SIM(d) (((SolverJobData *)(d))->simulation)

// 0x830ac400 "Create Simulation Islands"
void SimulationManager::CreateSimulationIslandTablesJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->CreateSimulationIslandTables(0); }
// 0x830ac410 "Contact Compiler"
void SimulationManager::ContactCompilerJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->ContactBatchBuild(); }
// 0x830ac418 "Joint Compiler"
void SimulationManager::JointCompilerJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->JointBatchBuild(); }
// 0x830ac420 "Drive Compiler"
void SimulationManager::DriveCompilerJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->DriveBatchBuild(); }
// 0x830ac428 "Iterations" (per island)
void SimulationManager::IslandIterationsJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->HorusPipelineIslandUsingIndirectJacobians(((SolverJobData *)d)->island); }
// 0x830ac438 "Iterations" (whole simulation)
void SimulationManager::IterationsJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->Horus_Pipeline(); }
// 0x830ac440 "Dynamic Update"
void SimulationManager::DynamicUpdateJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->BatchIntegrator(); }
// 0x830ac448 "Contact Spies"
void SimulationManager::ContactSpiesJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->SpyContactJacobians(); }
// 0x830ac450 "Joint Spies"
void SimulationManager::JointSpiesJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->SpyJointJacobians(); }
// 0x830ac458 "Drive Spies"
void SimulationManager::DriveSpiesJob(unsigned int d, unsigned int, unsigned int, unsigned int)
{ JOB_SIM(d)->SpyDriveJacobians(); }

} }
