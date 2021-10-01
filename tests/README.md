# Testing
Blimp uses unit testing via [Catch2](https://github.com/catchorg/Catch2).
You can contribute tests to the test.cpp file. Add necessary reference images to the `test_images` directory under the corresponding bit depth directory.

## Running Tests
1. Run `./buildTests.sh`
2. Execute binary `./tests`

## Adding new test images
You can use the include `test_generator.cpp` code to generate new test images. HOWEVER, you will need to manually verify the integrity of each test image (since they are generated with Blimp itself). In the future a third party BMP tool should be used to generate each test image to avoid manually checking each test image.
