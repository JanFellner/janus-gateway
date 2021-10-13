#include <glib.h>
#include "rtp.h"
#include "mutex.h"

/*! \brief This is a helper object that is used to pass publisher related data to the methods of the vp8_remb_simulcast implementation */
typedef struct janus_vp8_remb_publisher {
	/*! \brief Unique ID of the publisher */
	gchar *user_id_str;
	/*! \brief Display name (just for fun) */
	gchar *display;
 	/*! \brief Pointer to the original publisher object, to be handed over in the callback isMultiCasting */
	void *p_original_publisher_object;
 	/*! \brief Callback that is called inside a mutex context to verify if the publisher is simulcasting */
	gboolean (*isMultiCasting)(void *p_original_publisher_object);
	/*! \brief Callback to request a PLI / FIR on the subscribers peerConnection */
	void (*sendPLI)(void *p_original_publisher_object, const char *reason);
} janus_vp8_remb_publisher;

/*! \brief This is a helper object that is used to pass subscriber related data to the methods of the vp8_remb_simulcast implementation */
typedef struct janus_vp8_remb_subscriber {
	/*! \brief Opaque pointer to the Janus core-level handle */
	void *gateway_handle;
	/*! \brief The last received remb value */
	guint32 last_bitrate;
	/*! \brief stores wether we have already received a valid bitrate from remb or not yet, the first valid remb value sets this to true even if the value then drops to 0 afterwards */
	gboolean last_bitrate_valid;
	/*! \brief Simulcasting context of the subscribers peer Connection */
	janus_rtp_simulcasting_context *p_sim_context;
	/*! \brief Mutex one has to lock to safely access the publishers feed */
	janus_mutex* p_feed_mutex;
	/*! \brief Pointer to the publisher of this subscription */
	janus_vp8_remb_publisher *p_feed;
} janus_vp8_remb_subscriber;

/*! \brief Entry point to allow remb based switching simulcast layers
 * This method needs to be called when the remb value has been aquired from the rtcp data
 * @param subscriber - the subscriber object we are handling
 * @param bitrate - the received REMB bitrate value on the peerConnection
 */
void janus_vp8_remb_simulcast_based_subscriber_simulcast_switching(janus_vp8_remb_subscriber *subscriber, uint32_t bitrate);

/*! \brief Reads the remb_adoption property from the handed over json property and sets it
 * accordingliy in the simulcasting_remb_context context, returns false if the property has had
 * an invalid value
 * @param config_property - the property that contains the remb_adoption config value
 * @param pRemb_context - struct where we set the config_property into
 * @returns false in case the property contained an invalid value, true in any other case (not set or valid configured)
 */
gboolean janus_vp8_remb_simulcast_get_remb_adoption_config(json_t *config_property, janus_rtp_simulcasting_remb_context *remb_context);
