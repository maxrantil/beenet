all:
	make -C src
	make -C max_beenet_improved_movement
	cp max_beenet_improved_movement/agent .
