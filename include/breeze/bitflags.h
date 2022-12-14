#ifndef _BREEZE_BITFLAGS_H_
#define _BREEZE_BITFLAGS_H_

#include <iostream>
#include <vector>

namespace Breeze
{

/**
 * \brief A templated class implementing that can be used with enum classes in order to create bitflags.
 *
 * This class can be used to create bitflags from any enum class whose members are all set to ascending powers
 * of 2 starting from one (eg 0x1, 0x2, 0x4, 0x8, 0x10, etc or 1 << 0, 1 << 2, 1 << 3, etc.).
 *
 * \tparam The enum class to use in creating the bit flags. The enum class's members must be powers of two.
 * Using anything other than an enum class designed in this way will result in undefined behavior. \tparam
 * Integer type. Defaults to int which allows for 16 different state values or booleans. Other values such as
 * long int can be used instead to allow for more states.
 */
template <typename T, typename int_type = int>
class BitFlag
{
public:
  /**
   * \brief Construct a new Bit Flag object
   *
   * \param numFlags Optionally include the number of flags. This corresponds to the number of members in the
   * enum class. A nice way to have this value is to end the enum class with a member named 'COUNT'.
   */
  BitFlag(int numFlags = 0) : m_flags(0), m_numFlags(numFlags) {}

  /**
   * \brief Construct a new Bit Flag object
   *
   * \param flag A flag whose initial value will be 'on'.
   * \param numFlags Optionally include the number of flags. This corresponds to the number of members in the
   * enum class. A nice way to have this value is to end the enum class with a member named 'COUNT'.
   */
  BitFlag(T flag, int numFlags = 0) : m_flags(static_cast<int_type>(flag)), m_numFlags(numFlags) {}

  /**
   * \brief Construct a new Bit Flag object
   *
   * \param flags Multiple flags whose initial value will be 'on'.
   * \param numFlags Optionally include the number of flags. This corresponds to the number of members in the
   * enum class. A nice way to have this value is to end the enum class with a member named 'COUNT'.
   */
  BitFlag(std::vector<T> flags, int numFlags = 0) : m_flags(0), m_numFlags(numFlags)
  {
    for (auto flag : flags)
      m_flags += static_cast<int_type>(flag);
  }

  /**
   * \brief Turn on a flag.
   *
   * \param flag The flag to turn on.
   */
  void setFlag(T flag) { m_flags |= static_cast<int_type>(flag); }

  /**
   * \brief Turn off a flag.
   *
   * \param flag The flag to turn off.
   */
  void unsetFlag(T flag) { m_flags &= ~static_cast<int_type>(flag); }

  /**
   * \brief Switch a flag's value.
   *
   * This method will turn off a currently active flag, and it will turn on a currently inactive flag.
   *
   * \param flag The flag to switch.
   */
  void flipFlag(T flag) { m_flags ^= static_cast<int_type>(flag); }

  /**
   * \brief Turn on multiple flags.
   *
   * \param flags The flags to turn on.
   */
  void setFlags(std::vector<T> flags)
  {
    for (auto flag : flags)
      setFlag(flag);
  }

  /**
   * \brief Turn off multiple flags.
   *
   * \param flags The flags to turn off.
   */
  void unsetFlags(std::vector<T> flags)
  {
    for (auto flag : flags)
      unsetFlag(flag);
  }

  /**
   * \brief Switch multiple flags' values.
   *
   * This method will turn off the currently active flags that were passed, and it will turn on the currently
   * inactive flags.
   *
   * \param flags The flags to switch.
   */
  void flipFlags(std::vector<T> flags)
  {
    for (auto flag : flags)
      flipFlag(flag);
  }

  /**
   * \brief Turn all flags on.
   *
   * This methods needs to know how many flags there are. If the number of flags was not passed in the
   * constructor, it must be passed here. If it is not, this method returns false.
   *
   * \param numFlags The total number of flags in the object. Only needs to be passed if it was not passed in
   * the constructor.
   */
  void setAllFlags(int numFlags = 0)
  {
    int count;

    if (numFlags)
      count = numFlags;

    else if (m_numFlags)
      count = m_numFlags;

    else
      throw;

    m_flags = 0;
    for (int_type i = 0; i < count; i++)
    {
      setFlag(static_cast<T>(1 << i));
    }
  }

  /**
   * \brief Turn off all flags.
   */
  void unsetAllFlags() { m_flags = 0; }

  /**
   * \brief Flip all flags to their opposite value.
   *
   * This methods needs to know how many flags there are. If the number of flags was not passed in the
   * constructor, it must be passed here. If it is not, this method returns false.
   *
   * \param numFlags The total number of flags in the object. Only needs to be passed if it was not passed in
   * the constructor.
   */
  void flipAllFlags(int numFlags = 0)
  {
    int count;

    if (numFlags)
      count = numFlags;

    else if (m_numFlags)
      count = m_numFlags;

    else
      throw;

    for (int_type i = 0; i < count; i++)
      flipFlag(static_cast<T>(i));
  }

  /**
   * \brief Determine whether or not a specific flag is on.
   *
   * \param flag The flag to check.
   * \return Whether or not the passed flag is turned on.
   */
  bool hasFlag(T flag) { return m_flags & static_cast<int_type>(flag); }

  /**
   * \brief Determines whether any of the passed flags are turned on.
   *
   * \param flags The flags to check.
   * \return true
   */
  bool hasAnyFlag(std::vector<T> flags)
  {
    bool result;
    for (auto flag : flags)
    {
      result = hasFlag(flag);
      if (result)
        return result;
    }
    return result;
  }

  /**
   * \brief Determines whether or not any flags are turned on.
   *
   * \return Whether or not there are any flags turned on.
   */
  bool hasAnyFlag() { return static_cast<bool>(m_flags); }

  /**
   * \brief Determines whether or not all passed flags are true.
   *
   * \param flags The flags to check.
   * \return True if all passed flags are turned on, otherwise false.
   */
  bool hasAllFlags(std::vector<T> flags)
  {
    bool result;
    for (auto flag : flags)
    {
      result = hasFlag(flag);
      if (!result)
        return result;
    }
    return result;
  }

  /**
   * \brief Determines whether or not all flags are turned on.
   *
   * This methods needs to know how many flags there are. If the number of flags was not passed in the
   * constructor, it must be passed here. If it is not, this method returns false.
   *
   * \param numFlags The total number of flags in the object. Only needs to be passed if it was not passed in
   * the constructor. \return True if all flags are turned on. Otherwise false.
   */
  bool hasAllFlags(int numFlags = 0)
  {
    int count; // Allocate memory

    // Set count according to the number of flags. Priorize the method argument.
    if (numFlags)
      count = numFlags;
    else if (m_numFlags)
      count = m_numFlags;
    else
      throw;

    // Determine the result;
    bool result;
    for (int_type i = 0; i < count; i++)
    {
      result = hasFlag(static_cast<T>(1 << i));
      if (!result)
        return result;
    }
    return result;
  }

private:
  int_type m_flags;        ///< The current activated flags, stored as an integer.
  unsigned int m_numFlags; ///< The number of flags. A value of 0 means the number of flags is unknown.
};

} // namespace Breeze

#endif // _BREEZE_BITFLAGS_H_