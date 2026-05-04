```mermaid
classDiagram
    class FuelTank {
        -String name
        -float currentGallons
        +takeFuel(amount)
        +sendStatus()
    }
    class FuelPump {
        -String name
        -FuelTank* tank1
        +update(currentTime)
    }
    class PumpAndTankInstances {
        <<file>>
        +FuelTank regularTanks[4]
        +FuelTank dieselTanks[4]
        +FuelTank premiumTanks[2]
        +FuelPump pumps[10]
    }
    
    FuelPump --> FuelTank : pulls from
    PumpAndTankInstances ..> FuelTank : instantiates
    PumpAndTankInstances ..> FuelPump : instantiates
```

#### 2. Pumping Logic (Sequence Diagram)
This illustrates the "inventory removed from respective tank" logic we implemented, including the immediate feedback loop.

```mermaid
sequenceDiagram
    participant P as FuelPump
    participant T as FuelTank
    participant S as Serial Monitor

    loop Every Update Interval
        P->>T: takeFuel(amount)
        Note over T: Decrement _currentGallons
        T-->>S: sendStatus (JSON: "OK" or "WARNING")
        T-->>P: fuel amount returned
        P-->>S: sendStatus (JSON: "PUMPING")
    end
```

#### 3. Tank Mapping (Flow Chart)
A quick view of the distribution logic we finalized.

```mermaid
graph LR
    subgraph Pumps
        P1[Pumps 1-4]
        P2[Pumps 5-8]
        P3[Pumps 9-10]
    end

    subgraph Tanks
        T1[(4x Regular)]
        T2[(4x Diesel)]
        T3[(2x Premium)]
    end

    P1 --> T1
    P2 --> T2
    P3 --> T3
```