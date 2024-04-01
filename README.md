# Rejent_Syn
Wielowątkowy program realizujący poniższe zadanie:  Książe Dipontelini zmarł. Książe miał wielu synów (N). Jego włości stanowią X * Y działek. Każdy z synów pragnie zawładnąć największą liczbą terenów. Rejent ma niemałą zagwostkę jak pogodzić zwaśnionych synów.  Stwórz symulację tej sytuacji zachowując następujące zasady:     
1.) istnieje jeden wątek Rejent oraz N wątków Syn
2.) każdy Syn ma dokładnie S szans na zdobycie terytorium - losuje w tym celu współrzędne miejsca w tablicy terytoriów: jeśli miejsce jest wolne zajmuje je, jeśli miejsce jest zajęte, traci tą szansę     
3.) kiedy wątek wykorzysta S szans wysyła sygnał (zmienna warunkowa) do Rejenta, który blokuje tablicę i zlicza liczbę terytoriów zdobytych przez syna i wypisuje zdobycze syna na ekranie     
4.) kiedy wszyscy synowie skończą działanie wątek Rejent zlicza ile terytoriów nie zostało zajętych i wypisuje tę informację na tablicy.
