src_dir="src/"
readme_file="README"
debug_file="compile.out"
lib_flags="-lpthread"
out_flags="-omultiply"
debug_flags="-g"

default: main comp_time

all : about main comp_time

debug : debug_comp comp_time

about : 
	@cat README

main :
	@g++ $(lib_flags) $(out_flags) $(src_dir)main.cpp > $(debug_file)

debug_comp :
	@g++ $(lib_flags) $(debug_flags) $(out_flags) $(src_dir)main.cpp > $(debug_file)

comp_time :
	@echo "Last compiled on: " >> $(debug_file)
	@date >> $(debug_file)

