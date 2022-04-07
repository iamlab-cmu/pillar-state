import argparse
import logging

from simple_zmq import SimpleZMQServer
from pillar_state_py import State


def door_handle_turned(state):
    return state.get_values_as_vec(['frame:door:handle:rotation'])[0] > 0


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.INFO)
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', type=str, default='127.0.0.1')
    parser.add_argument('--port', type=str, default='5555')
    args = parser.parse_args()

    logging.info('Starting server on {}:{}'.format(args.ip, args.port))
    symbol_server = SimpleZMQServer(args.ip, args.port)

    while True:
        logging.info('Waiting for request...')
        state = State.create_from_serialized_string(symbol_server.recv())

        logging.info('Got state')
        symbol = door_handle_turned(state)

        logging.info('Symbol evaluated to {}'.format(symbol))
        symbol_server.rep(symbol)
