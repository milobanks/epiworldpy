import epiworldpy as epiworld

def test_seir_simple():
    covid19 = epiworld.ModelSEIR(
        name              = 'covid-19',
        n                 = 10000,
        prevalence        = .01,
        contact_rate      = 2.0,
        transmission_rate = .1,
        incubation_days   = 7.0,
        recovery_rate     = 0.14
    )

    covid19.run(100, 223)
    