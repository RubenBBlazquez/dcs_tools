import attr

from ui.components.json_menu_composer import DCSMenuParser
from ui.components.udp_messages_subscribers.base import UdpMessagesSubscriber
from ui.components.udp_methods import get_parsed_udp_json

TERRAIN_UDP_KEY = "terrain"

@attr.s(auto_attribs=True)
class TerrainUdpMessagesSubscriber(UdpMessagesSubscriber):
    """
    Subscriber for UDP messages related to terrain information.

    This class processes incoming UDP messages and extracts terrain-related
    information to update the relevant component. It is specifically designed
    to handle messages containing data under the defined terrain key.

    :ivar dcs_menu_parser_component: Component responsible for parsing DCS menu
        data with the ability to handle terrain updates.
    :type dcs_menu_parser_component: DCSMenuParser
    """
    dcs_menu_parser_component: DCSMenuParser

    def process_udp_message(self, message: str):
        udp_data = get_parsed_udp_json(message)

        if not udp_data and TERRAIN_UDP_KEY not in udp_data:
            return

        self.dcs_menu_parser_component._terrain = udp_data[TERRAIN_UDP_KEY]