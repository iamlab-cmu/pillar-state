from pillar_state import State


def test_nominal():
    state = State()

    state.update_property('object0/scalar', 9.999)
    state.update_property('object1/array', [1.111, 2.222, 3.333])
    state.update_property('object1/scalar', 4.444)
    state.update_property('object2/prop0/scalar', -22.222)
    state.update_property('object2/prop0/array', [-8.888, 9.999])
    state.update_property('object2/prop1/scalar', 7.777)
    state.update_property('object2/prop2/array', [123.123])

    # TODO(jacky): this doesn't seem to print anything?
    print('What does it look like to print out state?')
    print(state)

    print('state.num_properties(): ', state.num_properties)
    print('state.num_dimensions(): ', state.num_dimensions)
    print('state.size(): ', state.size)

    num_state_properties_gt = 7
    num_state_dimensions_gt = 10

    assert state.num_properties == num_state_properties_gt
    assert state.num_dimensions == num_state_dimensions_gt
    assert state.size == num_state_dimensions_gt

    # Now clear the state
    state.clear()

    print('What happens after running clear()?')
    print('state.num_properties(): ', state.num_properties)
    print('state.num_dimensions(): ', state.num_dimensions)
    print('state.size(): ', state.size)

    assert state.num_properties == 0
    assert state.num_dimensions == 0
    assert state.size == 0


def test_print_empty():
    state = State()

    print('What does it look like to print an empty state?')
    print(state)

    # TODO(jacky): implement after Property is exposed
    # PillarMsg::Property prop
    # state.update_property('empty_property', prop)

    # print('What does it look like to print an empty state property?')
    # print(state)

    assert True


def test_variance():
    state = State()

    state.update_property('object0/scalar', 1.111, 0.555)
    state.update_property('object0/array', [1.111, 2.222], [0.111, 0.222, 0.333, 0.444])

    print(state)


def test_state_from_yaml_file():
    pillar_env_yaml_path = 'test/env_3room_state.yaml'
    print('Reading in: ', pillar_env_yaml_path)
    state = State.create_from_yaml_file(pillar_env_yaml_path)
    print(state)

    door_yaml_path = 'test/door_state.yaml'
    print('Reading in: ', door_yaml_path)
    state = State.create_from_yaml_file(door_yaml_path)
    print(state)

    assert True


def test_literals_and_nodes():
    door_yaml_path = 'test/door_state.yaml'
    state = State.create_from_yaml_file(door_yaml_path)

    literal_names_abc_gt = ['desk', 'frame', 'frame:door', 'frame:door:handle']
    literal_names = state.literals()

    # Copy and sort
    literal_names_abc = list(literal_names)
    literal_names_abc.sort()

    print('State literals:')
    for n in literal_names_abc:
        print(' -> ', n)
    assert literal_names_abc == literal_names_abc_gt

    # Root nodes
    root_nodes_abc_gt = ['calibration_array', 'constants', 'desk', 'frame']
    root_nodes = state.root_nodes()

    # Copy and sort
    root_nodes_abc = list(root_nodes)
    root_nodes_abc.sort()

    print('Root nodes:')
    for n in root_nodes_abc:
        print(' -> ', n)
    assert root_nodes_abc == root_nodes_abc_gt

    # Get property names of a single literal
    # Note that this should NOT return any properties from any child literals
    frame_door_literal_props_abc_gt = [
        'frame:door:color',
        'frame:door:pose/position',
        'frame:door:pose/quaternion'
    ]
    frame_door_literal_props = state.get_literal_prop_names('frame:door')
    frame_door_literal_props_abc = list(frame_door_literal_props)
    frame_door_literal_props_abc.sort()

    print('Properties of literal frame:door:')
    for n in frame_door_literal_props_abc:
        print(' -> ', n)

    assert frame_door_literal_props_abc == frame_door_literal_props_abc_gt


def test_serialize_and_deserialize():
    pillar_env_yaml_path = 'test/env_3room_state.yaml'
    state = State.create_from_yaml_file(pillar_env_yaml_path)

    ser = state.get_serialized_string()
    new_state = State.create_from_serialized_string(ser)

    assert state.num_properties == new_state.num_properties
    assert state.num_dimensions == new_state.num_dimensions


def test_read_vec_values():
    pillar_env_yaml_path = 'test/env_3room_state.yaml'
    state = State.create_from_yaml_file(pillar_env_yaml_path)

    prop_names = ['wall1:length', 'wall1:pose2d', 'landmark2:pose2d']

    prop_sizes = state.get_prop_sizes(prop_names)
    assert prop_sizes['wall1:length'] == 1
    assert prop_sizes['wall1:pose2d'] == 2
    assert prop_sizes['landmark2:pose2d'] == 2

    vec_idxs = state.get_vec_idxs(prop_names)
    assert vec_idxs['wall1:length'][0] == 0
    assert vec_idxs['wall1:length'][1] == 1
    assert vec_idxs['wall1:pose2d'][0] == 1
    assert vec_idxs['wall1:pose2d'][1] == 3
    assert vec_idxs['landmark2:pose2d'][0] == 3
    assert vec_idxs['landmark2:pose2d'][1] == 5

    vec_values = state.get_values_as_vec(prop_names)
    gt_values = [80, 40, 0, 23, 2]
    for i in range(len(vec_values)):
        assert vec_values[i] == gt_values[i]

    vec_value_names = state.get_value_names_as_vec(prop_names)
    gt_value_names = ['', 'x', 'y', 'x', 'y']
    for i in range(len(vec_value_names)):
        assert vec_value_names[i] == gt_value_names[i]


def test_write_vec_values():
    pillar_env_yaml_path = 'test/env_3room_state.yaml'
    state = State.create_from_yaml_file(pillar_env_yaml_path)

    prop_names = ['wall1:length', 'wall1:pose2d', 'landmark2:pose2d']

    vec_values = state.get_values_as_vec(prop_names)
    for i in range(len(vec_values)):
        vec_values[i] *= 2

    state.set_values_from_vec(prop_names, vec_values)

    new_vec_values = state.get_values_as_vec(prop_names)
    for i in range(len(vec_values)):
        assert new_vec_values[i] == vec_values[i]
