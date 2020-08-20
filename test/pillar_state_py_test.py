from pillar_state_py import State


if __name__ == "__main__":
    yaml_path = 'test/door_state.yaml'

    s = State.create_from_yaml_file(yaml_path)

    import IPython; IPython.embed(); exit(0)