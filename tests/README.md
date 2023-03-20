# Testy dla projektu *Barriers*.


1. Testy należy uruchamiać z uprawnieniami użytkownika.
1. Przed rozpoczęciem testów w katalogu `tests` uruchom
```
	make
```
1. Skrypty uruchamiające kolejne testy znajdują się w katalogu `tests/tests`. Skrypty należy wywoływać z katalgu `tests` np.
```
	./tests/t2.sh
```
1. Innym sposobem wykonania, który sprawdza poprawność wyniku, jest użycie skryptu `run_test.sh` np.
```
	./run_test.sh tests/t2.sh
```
1. Do uruchomienia wszystkich testów służy `run_all.sh`
```
	./run_all.sh
```
1. Część testów wykorzystuje mechanizmy `assert`. W konsekwencji niepowodzenie testu może skutkować `coredump`.
1. Test `t5.sh` wymaga wyłączenia serwera w czasie trwania testu. Test prosi o to we właściwym momencie wypisując stosowny komunikat. Zatrzymanie serwera wymaga uprawnień administratora.
