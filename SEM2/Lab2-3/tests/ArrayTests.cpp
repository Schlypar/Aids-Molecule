#ifndef CATCH_CONFIG_MAIN
    #define CATCH_CONFIG_MAIN
#endif

#include "moduleTest.h"

LogPriority Logger::priority = FatalPriority;

#include "Sequence/Array.h"

TEST_CASE("Default constructor creates an array of size 1") {
  Array<int> arr;
  REQUIRE(arr.GetLength() == 1);
  REQUIRE(arr[0] == 0);
}

TEST_CASE("Array can be constructed from a raw array") {
  int raw_arr[3] = {1, 2, 3};
  Array<int> arr(raw_arr, 3);
  REQUIRE(arr.GetLength() == 3);
  REQUIRE(arr[0] == 1);
  REQUIRE(arr[1] == 2);
  REQUIRE(arr[2] == 3);
}

TEST_CASE("Array can be constructed with a specified size") {
  Size arrSize = 5;
  Array<int> arr(arrSize);
  REQUIRE(arr.GetLength() == 5);
  for (Index i = 0; i < 5; i++) {
    REQUIRE(arr[i] == 0);
  }
}

TEST_CASE("Array can be copied from another array") {
  Size originalSize = 3;
  Array<int> original(originalSize);
  original[0] = 1;
  original[1] = 2;
  original[2] = 3;

  Array<int> copied(original);

  REQUIRE(copied.GetLength() == 3);
  REQUIRE(copied[0] == 1);
  REQUIRE(copied[1] == 2);
  REQUIRE(copied[2] == 3);
}

TEST_CASE("Array can be moved from another array") {
  Size originalSize = 3;
  Array<int> original(originalSize);
  original[0] = 1;
  original[1] = 2;
  original[2] = 3;

  Array<int> moved(std::move(original));

  REQUIRE(original.GetLength() == 0);

  REQUIRE(moved.GetLength() == 3);
  REQUIRE(moved[0] == 1);
  REQUIRE(moved[1] == 2);
  REQUIRE(moved[2] == 3);
}

TEST_CASE("Array can be constructed with a parameter pack") {
  Array<int> arr(1, 2, 3, 4, 5);
  REQUIRE(arr.GetLength() == 5);
  REQUIRE(arr[0] == 1);
  REQUIRE(arr[1] == 2);
  REQUIRE(arr[2] == 3);
  REQUIRE(arr[3] == 4);
  REQUIRE(arr[4] == 5);
}

TEST_CASE("Array Realloc", "[Array]")
{
  Size arrSize = 5;
  Array<int> arr(arrSize);
  arr.Set(0, 1);
  arr.Set(1, 2);
  arr.Set(2, 3);
  arr.Set(3, 4);
  arr.Set(4, 5);

  SECTION("Realloc with larger size")
  {
      arr.Realloc(10);

      REQUIRE(arr.Get(0) == 1);
      REQUIRE(arr.Get(1) == 2);
      REQUIRE(arr.Get(2) == 3);
      REQUIRE(arr.Get(3) == 4);
      REQUIRE(arr.Get(4) == 5);

      // Check that the new elements are initialized to the default value of int (0)
      REQUIRE(arr.Get(5) == 0);
      REQUIRE(arr.Get(6) == 0);
      REQUIRE(arr.Get(7) == 0);
      REQUIRE(arr.Get(8) == 0);
      REQUIRE(arr.Get(9) == 0);
  }

  SECTION("Realloc with smaller size")
  {
      arr.Realloc(3);

      REQUIRE(arr.Get(0) == 1);
      REQUIRE(arr.Get(1) == 2);
      REQUIRE(arr.Get(2) == 3);
  }

  SECTION("Realloc with same size")
  {
      arr.Realloc(5);

      REQUIRE(arr.Get(0) == 1);
      REQUIRE(arr.Get(1) == 2);
      REQUIRE(arr.Get(2) == 3);
      REQUIRE(arr.Get(3) == 4);
      REQUIRE(arr.Get(4) == 5);
  }

  SECTION("Realloc with negative size")
  {
      REQUIRE_THROWS_AS(arr.Realloc(-1), Exception);
  }
}

TEST_CASE("Array Set and Get", "[Array]")
{
  Size arrSize = 5;
  Array<int> arr(arrSize);

  SECTION("Set and Get within range")
  {
      arr.Set(0, 1);
      arr.Set(1, 2);
      arr.Set(2, 3);
      arr.Set(3, 4);
      arr.Set(4, 5);

      REQUIRE(arr.Get(0) == 1);
      REQUIRE(arr.Get(1) == 2);
      REQUIRE(arr.Get(2) == 3);
      REQUIRE(arr.Get(3) == 4);
      REQUIRE(arr.Get(4) == 5);
  }

  SECTION("Set and Get out of range")
  {
      REQUIRE_THROWS_AS(arr.Set(5, 6), Exception);
      REQUIRE_THROWS_AS(arr.Set(-1, 0), Exception);
      REQUIRE_THROWS_AS(arr.Get(5), Exception);
      REQUIRE_THROWS_AS(arr.Get(-1), Exception);
  }
}

TEST_CASE("Array assignment operators", "[Array]") {

  SECTION("Copying operator") {
      // Create an Array to copy from
      Size arr1Size = 3;
      Array<int> arr1(arr1Size);
      arr1.Set(0, 1);
      arr1.Set(1, 2);
      arr1.Set(2, 3);

      // Create an Array to copy to and then copy arr1 into it
      Array<int> arr2;
      arr2 = arr1;

      // Check that arr2 was copied correctly
      REQUIRE(arr2.GetLength() == 3);
      REQUIRE(arr2.Get(0) == 1);
      REQUIRE(arr2.Get(1) == 2);
      REQUIRE(arr2.Get(2) == 3);
  }

  SECTION("Moving operator") {
      // Create an Array to move from
      Size arr1Size = 3;
      Array<int> arr1(arr1Size);
      arr1.Set(0, 1);
      arr1.Set(1, 2);
      arr1.Set(2, 3);

      // Create an Array to move to and then move arr1 into it
      Array<int> arr2;
      arr2 = std::move(arr1);

      // Check that arr2 was moved correctly
      REQUIRE(arr2.GetLength() == 3);
      REQUIRE(arr2.Get(0) == 1);
      REQUIRE(arr2.Get(1) == 2);
      REQUIRE(arr2.Get(2) == 3);

      // Check that arr1 was left in a valid but unspecified state
      REQUIRE(arr1.GetLength() == 0);
  }

}

