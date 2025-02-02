// athena
#include <athena/athena.hpp>
#include <athena/hydro/hydro.hpp>
#include <athena/parameter_input.hpp>

// canoe
#include <configure.hpp>

// application
#include <application/application.hpp>
#include <application/exceptions.hpp>

// harp
#include "harp/radiation.hpp"
#include "harp/radiation_band.hpp"

// inversion
#include "inversion/inversion.hpp"

// snap
#include "snap/decomposition/decomposition.hpp"
#include "snap/implicit/implicit_solver.hpp"
#include "snap/thermodynamics/thermodynamics.hpp"
#include "snap/turbulence/turbulence_model.hpp"

// microphysics
#include "microphysics/microphysics.hpp"

// c3m
#include "c3m/chemistry.hpp"

// tracer
#include "tracer/tracer.hpp"

// n-body
#include "nbody/particles.hpp"

// astro
#include "astro/celestrial_body.hpp"

// canoe
#include "impl.hpp"
#include "index_map.hpp"
#include "virtual_groups.hpp"

MeshBlock::Impl::Impl(MeshBlock *pmb, ParameterInput *pin) : pmy_block_(pmb) {
  Application::Logger app("main");

  du.NewAthenaArray(NHYDRO, pmb->ncells3, pmb->ncells2, pmb->ncells1);

  // decomposition
  pdec = std::make_shared<Decomposition>(pmb);

  // implicit methods
  phevi = std::make_shared<ImplicitSolver>(pmb, pin);

  // microphysics
  pmicro = std::make_shared<Microphysics>(pmb, pin);

  // radiation
  prad = std::make_shared<Radiation>(pmb, pin);

  // chemistry
  pchem = std::make_shared<Chemistry>(pmb, pin);

  // tracer
  ptracer = std::make_shared<Tracer>(pmb, pin);

  // turbulence
  pturb = TurbulenceFactory::Create(pmb, pin);

  // inversion queue
  all_fits = InversionsFactory::Create(pmb, pin);

  // particle queue
  all_particles = ParticlesFactory::Create(pmb, pin);

  // scheduler
  scheduler = SchedulerFactory::Create(pmb, pin);

  // planet
  planet = std::make_shared<CelestrialBody>(pin);

  // distance to parent star
  stellar_distance_au_ = pin->GetOrAddReal("problem", "distance_au", 1.);
  app->Log("Stellar distance = " + std::to_string(stellar_distance_au_) +
           " au");
}

MeshBlock::Impl::~Impl() {}

void MeshBlock::Impl::MapScalarsConserved(AthenaArray<Real> &s) {
  if (NCLOUD > 0) pmicro->u.InitWithShallowSlice(s, 4, 0, NCLOUD);
}
