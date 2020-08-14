#ifndef HIGHSCOREJSON_H
#define HIGHSCOREJSON_H
#include <vector>
#include <string>
#include <QFile>
/**
 * @pre A json-file must be available to be read and written to.
 * @brief SaveJson Saves the data from the game to scores.json -file
 * @param points The amount of points gained by the player
 * @param saved The number of ships the player has saved
 * @param name The name of the player.
 * @param time Time elapsed while playing the game.
 * @post the data has been saved to the json-file
 * @return
 */
void SaveJson(int points, int saved, QString name, QString time);

/**
 * @pre A json-file must be available to be read.
 * @brief ReadJson Reads the scores.json -file and returns the contents in a vector.
 * @return A vector of string-vectors containing the saved scores.
 */
std::vector<std::vector <std::string > > ReadJson();

#endif // HIGHSCOREJSON_H
