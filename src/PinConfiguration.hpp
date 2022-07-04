/**
 * @file PinConfiguration.hpp
 * @brief Default pin configuration used throughout the program.
 *
 * Could later be done through arguments to give full versatility.
 */
#ifndef FLIGHTCOMPUTER_PINCONFIGURATION_HPP
#define FLIGHTCOMPUTER_PINCONFIGURATION_HPP

#define LoRa_SCK 5 /**< @brief Used by LoRa module */
#define LoRa_CS 18 /**< @brief Used by LoRa module */
#define LoRa_MISO 19 /**< @brief Used by LoRa module */
#define LoRa_MOSI 27 /**< @brief Used by LoRa module */
#define LoRa_DIO0 26 /**< @brief Used by LoRa module */
#define LoRa_RST 14 /**< @brief Used by LoRa module */

#define SD_SCK 12 /**< @brief SD clock pin */
#define SD_CS 13 /**< @brief SD chip select pin */
#define SD_MISO 32 /**< @brief SD miso pin */
#define SD_MOSI 33 /**< @brief SD mosi pin */

#define GPS_RX 36 /**< @brief GPS rx pin */
#define GPS_TX 37 /**<@brief GPS tx pin */

#endif //FLIGHTCOMPUTER_PINCONFIGURATION_HPP
