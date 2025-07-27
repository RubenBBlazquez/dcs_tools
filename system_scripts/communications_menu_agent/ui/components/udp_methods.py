import json
import socket
import threading
from json import JSONDecodeError
from typing import Optional

import attr

from ui.components.udp_messages_subscribers.base import UdpMessagesSubscriber, DummyUdpMessagesSubscriber


def get_parsed_udp_json(udp_message: str) -> Optional[dict]:
    try:
        return json.loads(udp_message)
    except JSONDecodeError:
        return None

@attr.s(auto_attribs=True)
class UDPMessagesManagement:
    """
    Handles UDP messages management by providing methods for starting and stopping
    a UDP listening loop.

    The `UDPMessagesManagement` class is responsible for managing a UDP listener
    that processes incoming UDP messages by calling user-provided callbacks. It
    allows starting and stopping the listener as needed.

    Attributes:
    -----------
    _subscribers: list[list[UdpMessagesSubscriber]]
        Subscribers to send the udp messages received

    """
    _subscribers: list[UdpMessagesSubscriber] = attr.ib(default=None)
    _udp_stop_event: threading.Event = attr.ib(init=False)
    _udp_thread: threading.Thread = attr.ib(init=False)

    def __attrs_post_init__(self):
        self._subscribers = self._subscribers or [DummyUdpMessagesSubscriber()]

    def stop_listening(self):
        self._udp_stop_event.set()

    def start_listening(self):
        self._udp_stop_event = threading.Event()
        self._udp_thread = threading.Thread(target=self._listen_udp_loop, daemon=True)
        self._udp_thread.start()

    def _listen_udp_loop(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind(("127.0.0.1", 55337))
        sock.settimeout(0.5)

        while not self._udp_stop_event.is_set():
            try:
                data, addr = sock.recvfrom(1024)
            except socket.timeout:
                continue
            except Exception as e:
                print(f"UDP error: {e}")
                break

            msg = data.decode()
            print(f"UDP received from {addr}: {msg}")

            for subscriber in self._subscribers:
                subscriber.process_udp_message(msg)

        sock.close()
        print("UDP listener stopped.")