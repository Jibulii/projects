#include "probabilitydrive.h"
#include "shipengine.hh"
#include "starsystem.hh"
#include "galaxy.hh"
#include <cassert>
#include <cmath>

namespace Common {

const int ProbabilityDrive::RANGE = 100;

//The speed of the ship varies a lot.
const int ProbabilityDrive::MIN_DISTANCE_PER_MOVEMENT = 1;
const int ProbabilityDrive::MAX_DISTANCE_PER_MOVEMENT = 100;

//The probabilitydrive can even gain health on startup.
const int ProbabilityDrive::MAX_DAMAGE_PER_START = 1;
const int ProbabilityDrive::MIN_DAMAGE_PER_START = -2;
const int ProbabilityDrive::MAX_HEALTH = 3;

ProbabilityDrive::ProbabilityDrive(std::shared_ptr<IGalaxy> galaxy)
    : ShipEngine(MAX_HEALTH), galaxy_(galaxy)
{
}

bool ProbabilityDrive::canStart() const
{
    //The engine starts 50% of the time.
    int randomizer = Common::randomMinMax(0,1);
    if (randomizer == 0)
        return false;
    return true;
}

void ProbabilityDrive::onStart()
{
    decreaseHealth(randomMinMax(MIN_DAMAGE_PER_START, MAX_DAMAGE_PER_START));
}

StarSystem::StarSystemVector
ProbabilityDrive::getValidDestinations(std::shared_ptr<StarSystem> source) const
{
    assert(source && "PropabilityDrive: No source given for valid destination query.");

    if (!galaxy_)
    {
        return StarSystem::StarSystemVector();
    }

    return galaxy_->getSystemsInRange(source, RANGE);
}

bool ProbabilityDrive::isValidTrip(std::shared_ptr<StarSystem> source,
                            std::shared_ptr<StarSystem> destination) const
{
    assert(source && "ProbabilityDrive: No source given for valid trip query.");
    assert(destination && "ProbabilityDrive: No destination given for valid trip query.");

    double distance = destination->getCoordinates().distanceTo(source->getCoordinates());

    return distance <= RANGE;
}

int ProbabilityDrive::requiredTurns(std::shared_ptr<StarSystem> source,
                         std::shared_ptr<StarSystem> destination) const
{
    assert(MIN_DISTANCE_PER_MOVEMENT > 0);

    double distance = destination->getCoordinates().distanceTo(source->getCoordinates());

    double requiredTurns = distance/(randomMinMax(MIN_DISTANCE_PER_MOVEMENT,
                                                  MAX_DISTANCE_PER_MOVEMENT));

    int turns = ceil(requiredTurns);

    if (turns == 0)
        turns=1;

    return ceil(turns);
}
}
