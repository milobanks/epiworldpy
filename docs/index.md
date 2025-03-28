# epiworldpy: Python bindings for epiworld


[![](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/pip.yaml/badge.svg)](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/pip.yaml)
[![](https://img.shields.io/pypi/v/epiworldpy.svg)](https://pypi.org/project/epiworldpy)

This is a python wrapper of the [`epiworld c++`
library](https://github.com/UofUEpiBio/epiworld/), an ABM simulation
engine. This is possible using the
[`pybind11`](https://pybind11.readthedocs.io/en/stable/) library (which
rocks!).

The `epiworld` module is already
<a href="https://github.com/UofUEpiBio/epiworldR"
target="_blank">implemented in R</a>.

# Installation

-   clone this repository
-   `pip install ./epiworldpy`

# API

You can find API documentation on the <a href="/api.html">API page</a>.

# Examples

## Basic

Here we show how to create a `SEIR` object and add terms to it. We will
use the following data:

``` python
# Loading the module
import epiworldpy as epiworld

# Create a SEIR model (susceptible, exposed, infectious, recovered), representing COVID-19.
covid19 = epiworld.ModelSEIRCONN(
  name              = 'covid-19',
  n                 = 10000,
  prevalence        = .01,
  contact_rate      = 2.0,
  transmission_rate = .1,
  incubation_days   = 7.0,
  recovery_rate     = 0.14
)

# Taking a look
covid19.print(False)
```

    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : Susceptible-Exposed-Infected-Removed (SEIR) (connected)
    Population size     : 10000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 0 (of 0)
    Number of viruses   : 1
    Last run elapsed t  : -
    Rewiring            : off

    Global events:
     - Update infected individuals (runs daily)

    Virus(es):
     - covid-19

    Tool(s):
     (none)

    Model parameters:
     - Avg. Incubation days : 7.0000
     - Contact rate         : 2.0000
     - Prob. Recovery       : 0.1400
     - Prob. Transmission   : 0.1000

    <epiworldpy._core.ModelSEIRCONN at 0x10b9099f0>

Let’s run it and to see what we get:

``` python
# Run for 100 days with a seed of 223.
covid19.run(100, 223)

# Print an overview.
covid19.print(False)
```

    _________________________________________________________________________
    Running the model...
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
     done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : Susceptible-Exposed-Infected-Removed (SEIR) (connected)
    Population size     : 10000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 100 (of 100)
    Number of viruses   : 1
    Last run elapsed t  : 22.00ms
    Last run speed      : 43.64 million agents x day / second
    Rewiring            : off

    Global events:
     - Update infected individuals (runs daily)

    Virus(es):
     - covid-19

    Tool(s):
     (none)

    Model parameters:
     - Avg. Incubation days : 7.0000
     - Contact rate         : 2.0000
     - Prob. Recovery       : 0.1400
     - Prob. Transmission   : 0.1000

    Distribution of the population at time 100:
      - (0) Susceptible :  9900 -> 7275
      - (1) Exposed     :   100 -> 269
      - (2) Infected    :     0 -> 292
      - (3) Recovered   :     0 -> 2164

    Transition Probabilities:
     - Susceptible  1.00  0.00  0.00  0.00
     - Exposed      0.00  0.85  0.15  0.00
     - Infected     0.00  0.00  0.86  0.14
     - Recovered    0.00  0.00  0.00  1.00

    <epiworldpy._core.ModelSEIRCONN at 0x10b9099f0>

We can now visualize the model’s compartments:

``` python
import numpy as np
import matplotlib.pyplot as plt

# Get the data from the database
history = covid19.get_db().get_hist_total()

# Extract unique states and dates
unique_states = np.unique(history['states'])
unique_dates = np.unique(history['dates'])

# Remove some data that will mess with scaling
unique_states = np.delete(unique_states, np.where(unique_states == 'Susceptible'))

# Initialize a dictionary to store time series data for each state
time_series_data = {state: [] for state in unique_states}

# Populate the time series data for each state
for state in unique_states:
  for date in unique_dates:
    # Get the count for the current state and date
    mask = (history['states'] == state) & (history['dates'] == date)
    count = history['counts'][mask][0]
    time_series_data[state].append(count)

# Start the plotting!
plt.figure(figsize=(10, 6))

for state in unique_states:
  plt.plot(unique_dates, time_series_data[state], label=state)

plt.xlabel('Day')
plt.ylabel('Count')
plt.title('COVID-19 SEIR Model Data')
plt.legend()
plt.grid(True)
plt.show()
```

<img
src="README_files/figure-markdown_strict/series-visualization-output-1.png"
id="series-visualization" />

We can get the effective reproductive number, over time, too:

``` python
reproductive_data = covid19.get_db().get_reproductive_number()

# Start the plotting!
plt.figure(figsize=(10, 6))

for virus_id, virus_data in enumerate(reproductive_data):
    average_rts = list()

    for date_data in virus_data:
        if not date_data:
            continue

        keys_array = np.array(list(date_data.values()), dtype=np.float64)
        average_rts.append(np.mean(keys_array))

    plt.plot(range(0, len(virus_data)-1), average_rts, label=f"Virus {virus_id}")

plt.xlabel('Date')
plt.ylabel('Effective Reproductive Rate')
plt.title('COVID-19 SEIR Model Effective Reproductive Rate')
plt.legend()
plt.grid(True)
plt.show()
```

<img
src="README_files/figure-markdown_strict/rt-visualization-output-1.png"
id="rt-visualization" />

Let’s do the same for generation time:

``` python
from collections import defaultdict

generation_time = covid19.get_db().get_generation_time()
agents = generation_time['agents']
viruses = generation_time['viruses']
times = generation_time['times']
gentimes = generation_time['gentimes']

# Data formatting
unique_viruses = np.unique(viruses)
data = defaultdict(lambda: defaultdict(list))

for agent, virus, time, gentime in zip(agents, viruses, times, gentimes):
    data[virus][time].append(gentime)

average_data = {virus: {} for virus in unique_viruses}

for virus, time_dict in data.items():
    for time, gentime_list in time_dict.items():
        average_data[virus][time] = np.mean(gentime_list)

# Plotting
plt.figure(figsize=(10, 6))
for virus, time_dict in average_data.items():
    times = sorted(time_dict.keys())
    gentimes = [time_dict[time] for time in times]
    plt.plot(times, gentimes, label=f'Virus {virus}')

plt.xlabel('Date')
plt.ylabel('Generation Time')
plt.title('COVID-19 SEIR Model Generation Time')
plt.legend()
plt.grid(True)
plt.show()
```

<img
src="README_files/figure-markdown_strict/gentime-visualization-output-1.png"
id="gentime-visualization" />

Epiworld records agent-agent interactions, and we can graph those too.
In the below example, we only track all cases stemming from a specific
index case, despite the model having a prevalence of 0.01.

``` python
import networkx as nx
from matplotlib.animation import FuncAnimation

transmissions = covid19.get_db().get_transmissions()
start = transmissions['source_exposure_dates']
end = transmissions['dates']
source = transmissions['sources']
target = transmissions['targets']
days = max(end)

graph = nx.Graph()
fig, ax = plt.subplots(figsize=(6,4))

# Animation function
to_track = { source[0] }
def update(frame):
    ax.clear()

    agents_involved_today = set()
    agents_relationships_we_care_about = []

    # Get only the agents involved in the current frame.
    for i in range(len(start)):
        if start[i] <= frame <= end[i]:
            agents_involved_today.add((source[i], target[i]))

    # Get only today's agents who have some connection to agents
    # we've seen before.
    for agent in agents_involved_today:
        if agent[0] in to_track or agent[1] in to_track:
            to_track.add(agent[0])
            to_track.add(agent[1])
            graph.add_edge(agent[0], agent[1])

    # Lay and space them out.
    pos = nx.kamada_kawai_layout(graph)

    options = {
        "with_labels": True,
        "node_size": 300,
        "font_size": 6,
        "node_color": "white",
        "edgecolors": "white",
        "linewidths": 1,
        "width": 1,
    }

    # Graph!
    nx.draw_networkx(graph, pos, **options)
    ax.set_title(f"COVID-19 SEIR Model Agent Contact (Day {frame})")

ani = FuncAnimation(fig, update, frames=int(days/3), interval=200, repeat=False)
plt.show()
```

<!-- I couldn't figure out a way to get Quarto to do animations correctly so we're
  hardcoding a GIF. -->

![](README_files/figure-markdown_strict/contact-visualization-output-1.gif)
