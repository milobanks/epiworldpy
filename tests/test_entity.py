import epiworldpy as epiworld

def dist_factory(start, end):
    def entity_to_agent_fun(entity, model):
            agents = model.get_agents()

            for i in range(start, end):
                entity.add_agent(agents[i], model)

    return entity_to_agent_fun

def test_entity():
    hypothetical = epiworld.ModelSIRCONN(
        name              = 'hypothetical',
        n                 = 10000,
        prevalence        = 0.01,
        contact_rate      = 10,
        transmission_rate = 0.1,
        recovery_rate     = 0.14
    )

    entity_1 = epiworld.Entity("Entity 1", dist_factory(0, 3000))
    entity_2 = epiworld.Entity("Entity 2", dist_factory(3000, 6000))
    entity_3 = epiworld.Entity("Entity 3", dist_factory(6000, 10000))

    hypothetical.add_entity(entity_1)
    hypothetical.add_entity(entity_2)
    hypothetical.add_entity(entity_3)

    hypothetical.run(100, 223)
