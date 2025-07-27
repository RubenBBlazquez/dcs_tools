from abc import ABC, abstractmethod


class UdpMessagesSubscriber(ABC):
    """
    Represents an abstract base class for subscribing and processing UDP messages.

    This class serves as a blueprint for creating UDP message subscribers. It defines
    an abstract method that subclasses must implement to provide custom processing
    logic for UDP messages. The `UdpMessagesSubscriber` is designed to be extended
    to handle UDP communication effectively.
    """

    @abstractmethod
    def process_udp_message(self, message: str):
        pass


class DummyUdpMessagesSubscriber(UdpMessagesSubscriber):
    """
    Represents a dummy implementation of a UDP messages subscriber.

    This class inherits from the UdpMessagesSubscriber and provides a placeholder
    implementation for processing UDP messages. It is intended for testing or
    situations where a functional implementation of the subscriber is not needed.
    """

    def process_udp_message(self, value):
        print("You are using a dummy implementation of UdpMessagesSubscriber")
