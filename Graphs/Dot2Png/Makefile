####################################################################################
# 				How to Use
# 
# (1) Generate images from .dot files
#   make
# (2) Use 'feh' to display / view the images
#   make view
#                                                                 COMP9024 24T2
####################################################################################
PROJ_ROOT_PATH = $(shell pwd)


DOT_FILES = $(shell find ./images -name "*.dot")

# images/*.dot ---> images/*.png
PNG_FILES = $(DOT_FILES:.dot=.png)

# create a directory  
$(shell mkdir -p images)


all: ${PNG_FILES}
	@echo "Generating *.png files from *.dot files"


images/%.png: images/%.dot
	@mkdir -p $(shell dirname $@)
	dot -T png $< -o $@ 


view:
	find . -name "*.png" | sort | xargs feh &


# clean all the files generated	
clean:	
	find . -name "*.png" | xargs rm -f
