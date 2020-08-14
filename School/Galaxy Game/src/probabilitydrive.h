#ifndef PROBABILITYDRIVE_H
#define PROBABILITYDRIVE_H

#include "shipengine.hh"
#include "igalaxy.hh"


namespace Common {

/**
 * @brief The ProbabilityDrive class
 * An additional engine type inspired from the novel
 * Hitchhiker's guide to the galaxy
 */
class ProbabilityDrive : public ShipEngine
{
public:

    /**
     * @brief RANGE Maximum distance between origin point and target
     */
    static const int RANGE;
    /**
     * @brief MIN_DISTANCE_PER_MOVEMENT Minimum distance in one eventcycle
     */
    static const int MIN_DISTANCE_PER_MOVEMENT;
    /**
     * @brief MAX_DISTANCE_PER_MOVEMENT Maximum distance in one eventcycle
     */
    static const int MAX_DISTANCE_PER_MOVEMENT;
    /**
     * @brief MAX_HEALTH Max health for probability drive engine
     */
    static const Health MAX_HEALTH;
    /**
     * @brief MAX_DAMAGE_PER_START Max damage that engine can experience at one time
     */
    static const Health MAX_DAMAGE_PER_START;
    /**
     * @brief MIN_DAMAGE_PER_START Min damage that engine can experience at one time
     */
    static const Health MIN_DAMAGE_PER_START;

    ProbabilityDrive(std::shared_ptr<IGalaxy> galaxy);

    /**
     * @brief canStart Checks if the engine can be started
     * @return 50/50 to return true or false
     * @post Exception guarantee: nothrow
     */
    virtual bool canStart() const;

    /**
     * @brief onStart decreases health random amount between min and max damage
     * @post Exception guarantee: basic
     */
    virtual void onStart();

    /**
     * @brief getValidDestinations get all destinations where can travel
     * @pre Parameter source must not be nullptr.
     * @post Exception guarantee: nothrow
     * @param source of the ship
     * @return all valid destinations
     */
    virtual StarSystem::StarSystemVector getValidDestinations(std::shared_ptr<StarSystem> source) const;

    /**
     * @brief isValidTrip checks if the travel can be done
     * @pre Parameter source must not be nullptr.
     * @pre Parameter destination must not be nullptr.
     * @post Exception guarantee: nothrow
     * @param source of the ship
     * @param destination starsystem where to be reached
     * @return bool if the destination can be reached
     */
    virtual bool isValidTrip(std::shared_ptr<StarSystem> source,
                            std::shared_ptr<StarSystem> destination) const;
    /**
     * @brief requiredTurns
     * @param source of the ship
     * @param destination starsystem where to be reached
     * @return amount of turns that the trip takes
     */
    virtual int requiredTurns(std::shared_ptr<StarSystem> source,
                             std::shared_ptr<StarSystem> destination) const;

private:
    std::shared_ptr<IGalaxy> galaxy_;
};

}

#endif // PROBABILITYDRIVE_H
