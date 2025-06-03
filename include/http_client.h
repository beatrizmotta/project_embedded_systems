
/**
 * @brief Sends a traffic infraction notification to the cloud server.
 *
 * @param plate         The license plate of the vehicle.
 * @param speed_kmh     The recorded speed of the vehicle (in km/h).
 * @param timestamp     The timestamp of the infraction in ISO 8601 format.
 * @param image_hash    The hash string representing the image captured.
 *
 * @return 0 on success, negative error code on failure.
 */
int send_infraction_notification(const char *plate, int speed_kmh, const char *timestamp, const char *image_hash);


