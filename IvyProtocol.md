# Control Center #

Messages triggered by the ControlCenter :

| Protocol | Params | Class | From | Description |
|:---------|:-------|:------|:-----|:------------|
| `Bus_([0-9]+) Start` | instance\_id | BusStartMsg | Bus | Sended by the bus process, give to all its instance number |
| `Bus id=([0-9]+) line=([0-9]+) pos=([0-9]+),([-]?[0-9]+) capacity=([0-9]+) speed=([0-9]+)` | int, int, int, int, int, int | BusPositionMsg | Bus | Send his position by giving the road id, and the distance on it. Positive distance means the bus is going away, and negative means it's coming back. And the capacity of the bus at the moment. Finally the instant speed.|
| `Station_([0-9]+) Start` | instance\_id | StationStartMsg | Station | Sended by the station process, give to all its instance number. |
| `gui createBus id=([0-9]+) passengers=([0-9]+) line=([0-9]+)` | int,int,int |  | GUI | Ask to the command center to create a bus. |
| Bus id= ([0-9]+) emeute | int |  | bus | Pb with passager |
| Bus id= ([0-9]+) panne|  int |  | bus | Pb with engine |
| Bus id= ([0-9]+) bouchon|  int |  | bus | Pb with engine |
| gui id=([0-9]+) acceleter | int |  | gui | Increase speed of the bus |
| gui id=([0-9]+) ralentir|  int |  | gui | Decrease speed of bus |
| gui id=([0-9]+) delete|  int |  | gui | Delete the bus |
| gui route\_id=([0-9]+) pbciruclation|  int |  | gui | Blocked road |
| `CC Station Path=([0-9]+):([0-9]+)` | int, int |  | gui/bus | Calculate a path between  two station. |
# Bus #

Messages triggered by the Bus :

| Protocol | Parameters | Callback Class | From | Description |
|:---------|:-----------|:---------------|:-----|:------------|
| `Bus_([0-9]+) id=([0-9]+) passengers=([0-9]+) line=([0-9]):(([0-9]+,[0-9]+;)*)` | instance\_id,int,int,int,string |  | CC | Initialise the bus process, with control center data. Send the internal bus id, its capacity, and the affected line. |
| `Station id= ([0-9]+) bus_id= ([0-9]+) len= ([0-9]+)` | int, int, int |  | Station | The station send his position, relative to the bus position on the common road portion. |
| gui id= ([0-9]+) emeute | int |  | gui | Pb with passager |
| gui id= ([0-9]+) panne|  int |  | gui | Pb with engine |
| gui id= ([0-9]+) bouchon |  int |  | gui | Pb with traffic |
| gui id= ([0-9]+) ok |  int |  | gui | traffic ok |

# Station #

Messages triggered by the Station :

| Protocol | Parameters | Callback Class | From | Description |
|:---------|:-----------|:---------------|:-----|:------------|
| `Station_([0-9]+) id=([0-9]+) road=([0-9]+) lines=([0-9]+(,[0-9]+)*) len=([0-9]+)` | instance\_id, int, int, string, int |  | CC | Synchronize the station process with the control center data. |
| gui getTimes station id= ([0-9]+)| int, int |  | gui | Ask information about station |

# GUI #

Messages triggered by the GUI :

| Protocol | Parameters | Callback Class | From | Description |
|:---------|:-----------|:---------------|:-----|:------------|
| `Station id= ([0-9]+) time=(( [0-9]+, [0-9]+, [0-9]+;)*)`  | int, [int, int]+ |  | Station | Send the information from bus traffic to the GUI, & CC. |

# Scenario #

We have 3 bus process, 3 stations, 3 lines.

| Event | Bus | Station | CC | GUI |
|:------|:----|:--------|:---|:----|
| Station 1 Start |  | `S1 >> Station_1234 Start` | `Station_1234 id=1 road=1 lines=1,2 len=100` |  |
| Station 2 Start |  | `S2 >> Station_4321 Start` |`Station_4321 id=2 road=2 lines=1,2 len=100` |  |
| Station 3 Start |  | `S3 >> Station_5678 Start` | `Station_5678 id=3 road=3 lines=1 len=100` |  |
| Station 4 Start |  | `S4 >> Station_1357 Start` | `Station_1357 id=4 road=8 lines=2 len=250` |  |
| Bus 1 Start | `B1 >> Bus_12134 Start` |  | `Bus_12134 id=1 passengers=50 line=1:1,300;2,300;3,300;4,300;` |  |
|  | `B1 >> Bus id=1 line=1 pos=1,0 capacity=50` | `S1 >> Station id=1 bus_id=1 len=100` |  |  |
| Bus 2 Start | `B2 >> Bus_34521 Start` |  | `Bus_34521 id=2 passengers=20 line=2:1,300;2,300;8,300;7,600;6,600;5,300;` |  |
|  | `B2 >> Bus id=2 line=2 pos=1,0 capacity=20 speed=50` | `S1 >> Station id=1 bus_id=1 len=100` |  |  |
| Bus 3 Start | `B3 >> Bus_56789 Start` |  | `Bus_56789 id=3 passengers=50 line=3:6,600;7,600;8,300;9,300;10,600;11,900;12,300;` |  |
|  | `B3 >> Bus id=3 line=3 pos=6,0 capacity=50` |  |  |  |
| B1 arrive to S1 | `B1 >> Bus id=1 line=1 pos=1,100 capacity=37 speed=0` |  |  |  |
| B2 arrive to S1 | `B2 >> Bus id=2 line=2 pos=1,100 capacity=19 speed=0` |  |  |  |
| B1 leave road 1 | `B1 >> Bus id=1 line=1 pos=2,0 capacity=37 speed=30` | `S2 >> Station id=2 bus_id=1 len=100` |  |  |
| B2 leave road 1 | `B2 >> Bus id=2 line=2 pos=2,0 capacity=19 speed=30` | `S2 >> Station id=2 bus_id=2 len=100` |  |  |
| B1 arrive to S2 | `B1 >> Bus id=1 line=1 pos=2,100 capacity=32 speed=0` |  |  |  |
| B2 arrive to S2 | `B2 >> Bus id=2 line=2 pos=2,100 capacity=15 speed=0` |  |  |  |
| B1 leave road 2 | `B1 >> Bus id=1 line=1 pos=3,0 capacity=32 speed=50` | `S3 >> Station id=3 bus_id=1 len=100` |  |  |
| B2 leave road 2 | `B2 >> Bus id=2 line=2 pos=8,0 capacity=15 speed=30` |  |  |  |
| B1 arrive to S3 | `B1 >> Bus id=1 line=1 pos=3,100 capacity=50 speed=0` |  |  |  |
| B2 leave road 8 | `B2 >> Bus id=2 line=2 pos=7,0 capacity=15 speed=90` | `S4 >> Station id=4 bus_id=2 len=250` |  |  |
|  |  |  |  |  |
| B4 is created | 2 `Bus_15678 Start` |  | 3 `Bus_15678 id=4 passengers=50 line=2:1,300;2,300;8,300;7,600;6,600;5,300;` | 1 `gui createBus line=2 capacity=50`|
|  | `B4 >> Bus id=4 line=2 pos=1,0 capacity=50` | `S1 >> Station id=1 bus_id=4 len=100` |  |  |