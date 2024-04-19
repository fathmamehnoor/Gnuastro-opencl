DEVICE = CL_DEVICE_TYPE_DEFAULT
export DEVICE

all: main

ifeq ($(DEVICE), CL_DEVICE_TYPE_CPU)
	gcc -DDEVICE=$(DEVICE) -DCL_TARGET_OPENCL_VERSION=120 -c gpu_utils.c -lgnuastro -lcfitsio -lOpenCL
	gcc -DCL_TARGET_OPENCL_VERSION=120 -c gpu_conv.c -lgnuastro -lcfitsio -lOpenCL
	gcc -DCL_TARGET_OPENCL_VERSION=120 main.c gpu_utils.o gpu_conv.o -o main -lgnuastro -lcfitsio -lOpenCL
else
	gcc -DDEVICE=$(DEVICE) -DCL_TARGET_OPENCL_VERSION=300 -c gpu_utils.c -lgnuastro -lcfitsio -lOpenCL
	gcc -DCL_TARGET_OPENCL_VERSION=300 -c gpu_conv.c -lgnuastro -lcfitsio -lOpenCL
	gcc -DCL_TARGET_OPENCL_VERSION=300 main.c gpu_utils.o gpu_conv.o -o main -lgnuastro -lcfitsio -lOpenCL
endif



run:
	./main

clean:
	rm -f main *.o

