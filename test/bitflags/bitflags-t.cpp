#include <breeze/bitflags.h>

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

/**
 * @brief Test for BitFlag class.
 * 
 * This test goes through every method at least once and tests everything with a variety of parameters. If you feel
 * that the test is inadequate, feel free to update and create a pull request. Also please report any bugs that
 * may appear through more testing conditions. When built, this test is automatically run when ctest is executable from 
 * the command line inside of the build directory.
 * 
 * @return 0 if test passes, -1 if test fails.
 */
int main()
{
  Breeze::BitFlag<TestClass> bf1(static_cast<int>(TestClass::count));
  Breeze::BitFlag<TestClass, int> bf2(TestClass::b);
  Breeze::BitFlag<TestClass, long int> bf3({ TestClass::a, TestClass::d });

  bf1.setFlag(TestClass::a);
  if (!bf1.hasFlag(TestClass::a))
    return -1;

  bf2.unsetFlag(TestClass::b);
  if (bf2.hasFlag(TestClass::b))
    return -1;

  bf3.flipFlag(TestClass::a);
  if (bf3.hasFlag(TestClass::a) && !bf3.hasFlag(TestClass::d))
    return -1;

  bf2.setFlags({ TestClass::a, TestClass::f, TestClass::e} );
  if (!bf2.hasFlag(TestClass::a) && !bf2.hasFlag(TestClass::f) && !bf2.hasFlag(TestClass::e))
    return -1;

  bf2.unsetFlags({ TestClass::a, TestClass::f, TestClass::e} );
  if (bf2.hasFlag(TestClass::a) && bf2.hasFlag(TestClass::f) && bf2.hasFlag(TestClass::e))
    return -1;

  bf1.flipFlags({TestClass::a, TestClass::c});
  if (bf1.hasFlag(TestClass::a) && !bf1.hasFlag(TestClass::c))
    return -1;

  bf2.unsetAllFlags();
  bf2.setFlag(TestClass::a);
  bf2.flipAllFlags(static_cast<int>(TestClass::count));
  if (bf2.hasFlag(TestClass::a))
    return -1;

  if (bf2.hasAllFlags({TestClass::b, TestClass::c}))
    return -1;

  if (!bf1.hasAnyFlag({TestClass::a, TestClass::c}))
    return 1;

  if (!bf3.hasAnyFlag())
    return -1;

  bf3.unsetAllFlags();
  if (bf3.hasAnyFlag())
    return -1;

  bf1.setFlags({TestClass::a, TestClass::c});
  if (!bf1.hasAllFlags({TestClass::a, TestClass::c}))
    return -1;

  bf1.setAllFlags();
  if (!bf1.hasAllFlags())
    return -1;

  bf1.unsetFlag(TestClass::a);
  if (bf1.hasAllFlags())
    return -1;

  return 0;
}