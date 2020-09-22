import argparse
import logging

from simple_zmq import SimpleZMQClient
from pillar_state_py import State


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.INFO)
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', type=str, default='127.0.0.1')
    parser.add_argument('--port', type=str, default='5555')
    args = parser.parse_args()

    logging.info('Binding to server on {}:{}'.format(args.ip, args.port))
    symbol_client = SimpleZMQClient(args.ip, args.port)

    door_yaml_path = 'test/door_state.yaml'
    logging.info('Loading {}'.format(door_yaml_path))
    state = State.create_from_yaml_file(door_yaml_path)

    handle_rotation_fqn = 'frame:door:handle:rotation'
    logging.info('Handle rotation: {}'.format(state.get_values_as_vec([handle_rotation_fqn])[0]))

    logging.info('Evaluating symbol...')
    symbol = symbol_client.send(state.get_serialized_string())
    logging.info('Symbol evaluated to {}'.format(symbol))

    new_handle_rot = 1
    logging.info('Changing handle rotation to {}'.format(new_handle_rot))
    state.set_values_from_vec([handle_rotation_fqn], [new_handle_rot])

    logging.info('Evaluating symbol...')
    symbol = symbol_client.send(state.get_serialized_string())
    logging.info('Symbol evaluated to {}'.format(symbol))
