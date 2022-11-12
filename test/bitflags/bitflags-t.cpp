#include "breeze/bitflags.h"

enum class TestClass
{
  a = 1 << 0,
  b = 1 << 1,
  c = 1 << 2,
  d = 1 << 3,
  e = 1 << 4, 
  f = 1 << 5,
  count = 6
};

int main()
{
  BitFlag<TestClass> testing(static_cast<int>(TestClass::count));

  testing.setFlag(TestClass::a);
  if (testing.flagOn(TestClass::a))
    return 0;

  return -1;
}