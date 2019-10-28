
#ifndef NUCLEUS_TEXT_STRING_VIEW_H_
#define NUCLEUS_TEXT_STRING_VIEW_H_

#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Types.h"

#include <algorithm>
#include <cstring>
#include <ostream>

namespace nu {

namespace detail {

constexpr auto stringLength(const char* text) -> StringLength {
  StringLength length = 0;
  for (; text[length]; ++length) {
  }
  return length;
}

}  // namespace detail

class StringView {
public:
  static StringLength kInvalidPosition;

  using ConstIterator = const Char*;

  constexpr StringView() noexcept : m_data{nullptr}, m_length{0} {}

  constexpr StringView(const StringView&) noexcept = default;
  constexpr StringView& operator=(const StringView&) noexcept = default;

  constexpr StringView(const char* text) noexcept
    : m_data{text}, m_length{detail::stringLength(text)} {}

  constexpr StringView(const char* text, StringLength length) noexcept
    : m_data{text}, m_length{length} {}

  constexpr auto begin() -> ConstIterator {
    return m_data;
  }

  constexpr auto end() -> ConstIterator {
    return m_data + m_length;
  }

  constexpr auto length() const noexcept -> StringLength {
    return m_length;
  }

  constexpr auto empty() const noexcept -> bool {
    return m_length == 0;
  }

  constexpr auto data() const noexcept -> const Char* {
    return m_data;
  }

  constexpr auto operator[](StringLength index) const noexcept -> const Char& {
    DCHECK(index < m_length);

    return m_data[index];
  }

  constexpr auto at(StringLength index) const -> const Char& {
    DCHECK(index < m_length);

    return m_data[index];
  }

#if 0
  static constexpr auto npos{static_cast<size_type>(-1)};

    constexpr void remove_prefix(const size_type _Count) noexcept { // strengthened
        // chop off the beginning
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mysize >= _Count, "cannot remove prefix longer than total size");
#endif  // _CONTAINER_DEBUG_LEVEL > 0
        _Mydata += _Count;
        _Mysize -= _Count;
    }

    constexpr void remove_suffix(const size_type _Count) noexcept { // strengthened
        // chop off the end
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mysize >= _Count, "cannot remove suffix longer than total size");
#endif  // _CONTAINER_DEBUG_LEVEL > 0
        _Mysize -= _Count;
    }

    constexpr void swap(basic_string_view& _Other) noexcept { // swap contents
        const basic_string_view _Tmp{_Other}; // note: std::swap is not constexpr
        _Other = *this;
        *this  = _Tmp;
    }

    constexpr size_type copy(_Out_writes_(_Count) _Elem* const _Ptr, size_type _Count, const size_type _Off = 0) const {
        // copy [_Off, _Off + Count) to [_Ptr, _Ptr + _Count)
        _Check_offset(_Off);
        _Count = _Clamp_suffix_size(_Off, _Count);
        _Traits::copy(_Ptr, _Mydata + _Off, _Count);
        return _Count;
    }

    _Pre_satisfies_(_Dest_size >= _Count) constexpr size_type
        _Copy_s(_Out_writes_all_(_Dest_size) _Elem* const _Dest, const size_type _Dest_size, size_type _Count,
            const size_type _Off = 0) const { // copy [_Off, _Off + _Count) to [_Dest, _Dest + _Count)
        _Check_offset(_Off);
        _Count = _Clamp_suffix_size(_Off, _Count);
        _Traits::_Copy_s(_Dest, _Dest_size, _Mydata + _Off, _Count);
        return _Count;
    }

    _NODISCARD constexpr basic_string_view substr(const size_type _Off = 0, size_type _Count = npos) const {
        // return a new basic_string_view moved forward by _Off and trimmed to _Count elements
        _Check_offset(_Off);
        _Count = _Clamp_suffix_size(_Off, _Count);
        return basic_string_view(_Mydata + _Off, _Count);
    }

    constexpr bool _Equal(const basic_string_view _Right) const noexcept { // test if *this and _Right are equal
        return _Traits_equal<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr int compare(const basic_string_view _Right) const
        noexcept { // compare [0, _Mysize) with _Right
        return _Traits_compare<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _N0, const basic_string_view _Right) const {
        // compare [_Off, _Off + _N0) with _Right
        return substr(_Off, _N0).compare(_Right);
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _N0, const basic_string_view _Right,
        const size_type _Roff, const size_type _Count) const {
        // compare [_Off, _Off + _N0) with _Right [_Roff, _Roff + _Count)
        return substr(_Off, _N0).compare(_Right.substr(_Roff, _Count));
    }

    _NODISCARD constexpr int compare(_In_z_ const _Elem* const _Ptr) const { // compare [0, _Mysize) with [_Ptr, <null>)
        return compare(basic_string_view(_Ptr));
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _N0, _In_z_ const _Elem* const _Ptr) const {
        // compare [_Off, _Off + _N0) with [_Ptr, <null>)
        return substr(_Off, _N0).compare(basic_string_view(_Ptr));
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _N0,
        _In_reads_(_Count) const _Elem* const _Ptr, const size_type _Count) const {
        // compare [_Off, _Off + _N0) with [_Ptr, _Ptr + _Count)
        return substr(_Off, _N0).compare(basic_string_view(_Ptr, _Count));
    }

#if _HAS_CXX20
    _NODISCARD constexpr bool starts_with(const basic_string_view _Right) const noexcept {
        // test if this instance starts with _Right
        const auto _Rightsize = _Right._Mysize;
        if (_Mysize < _Rightsize) {
            return false;
        }
        return _Traits::compare(_Mydata, _Right._Mydata, _Rightsize) == 0;
    }

    _NODISCARD constexpr bool starts_with(const _Elem _Right) const noexcept {
        // test if this instance starts with _Right
        return !empty() && _Traits::eq(front(), _Right);
    }

    _NODISCARD constexpr bool starts_with(const _Elem* const _Right) const noexcept { // strengthened
        // test if this instance starts with _Right
        return starts_with(basic_string_view(_Right));
    }

    _NODISCARD constexpr bool ends_with(const basic_string_view _Right) const noexcept {
        // test if this instance ends with _Right
        const auto _Rightsize = _Right._Mysize;
        if (_Mysize < _Rightsize) {
            return false;
        }
        return _Traits::compare(_Mydata + (_Mysize - _Rightsize), _Right._Mydata, _Rightsize) == 0;
    }

    _NODISCARD constexpr bool ends_with(const _Elem _Right) const noexcept {
        // test if this instance ends with _Right
        return !empty() && _Traits::eq(back(), _Right);
    }

    _NODISCARD constexpr bool ends_with(const _Elem* const _Right) const noexcept { // strengthened
        // test if this instance ends with _Right
        return ends_with(basic_string_view(_Right));
    }
#endif  // _HAS_CXX20

    _NODISCARD constexpr size_type find(const basic_string_view _Right, const size_type _Off = 0) const
        noexcept { // look for _Right beginning at or after _Off
        return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr size_type find(const _Elem _Ch, const size_type _Off = 0) const noexcept {
        // look for _Ch at or after _Off
        return _Traits_find_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept { // strengthened
        // look for [_Ptr, _Ptr + _Count) beginning at or after _Off
        return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Count);
    }

    _NODISCARD constexpr size_type find(_In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const
        noexcept { // strengthened
        // look for [_Ptr, <null>) beginning at or after _Off
        return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr));
    }

    _NODISCARD constexpr size_type rfind(const basic_string_view _Right, const size_type _Off = npos) const noexcept {
        // look for _Right beginning before _Off
        return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr size_type rfind(const _Elem _Ch, const size_type _Off = npos) const noexcept {
        // look for _Ch before _Off
        return _Traits_rfind_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type rfind(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept { // strengthened
        // look for [_Ptr, _Ptr + _Count) beginning before _Off
        return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Count);
    }

    _NODISCARD constexpr size_type rfind(_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const
        noexcept { // strengthened
        // look for [_Ptr, <null>) beginning before _Off
        return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr));
    }

    _NODISCARD constexpr size_type find_first_of(const basic_string_view _Right,
        const size_type _Off = 0) const noexcept { // look for one of _Right at or after _Off
        return _Traits_find_first_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_of(const _Elem _Ch, const size_type _Off = 0) const noexcept {
        // look for _Ch at or after _Off
        return _Traits_find_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_first_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept { // strengthened
        // look for one of [_Ptr, _Ptr + _Count) at or after _Off
        return _Traits_find_first_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_of(_In_z_ const _Elem* const _Ptr,
        const size_type _Off = 0) const noexcept { // strengthened
        // look for one of [_Ptr, <null>) at or after _Off
        return _Traits_find_first_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_of(const basic_string_view _Right, const size_type _Off = npos) const
        noexcept { // look for one of _Right before _Off
        return _Traits_find_last_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_of(const _Elem _Ch, const size_type _Off = npos) const noexcept {
        // look for _Ch before _Off
        return _Traits_rfind_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_last_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept { // strengthened
        // look for one of [_Ptr, _Ptr + _Count) before _Off
        return _Traits_find_last_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_of(_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const
        noexcept { // strengthened
        // look for one of [_Ptr, <null>) before _Off
        return _Traits_find_last_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_not_of(const basic_string_view _Right,
        const size_type _Off = 0) const noexcept { // look for none of _Right at or after _Off
        return _Traits_find_first_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_not_of(const _Elem _Ch, const size_type _Off = 0) const noexcept {
        // look for any value other than _Ch at or after _Off
        return _Traits_find_not_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_first_not_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept { // strengthened
        // look for none of [_Ptr, _Ptr + _Count) at or after _Off
        return _Traits_find_first_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_not_of(_In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const
        noexcept { // strengthened
        // look for none of [_Ptr, <null>) at or after _Off
        return _Traits_find_first_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_not_of(const basic_string_view _Right,
        const size_type _Off = npos) const noexcept { // look for none of _Right before _Off
        return _Traits_find_last_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_not_of(const _Elem _Ch, const size_type _Off = npos) const noexcept {
        // look for any value other than _Ch before _Off
        return _Traits_rfind_not_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_last_not_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept { // strengthened
        // look for none of [_Ptr, _Ptr + _Count) before _Off
        return _Traits_find_last_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_not_of(_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const
        noexcept { // strengthened
        // look for none of [_Ptr, <null>) before _Off
        return _Traits_find_last_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr bool _Starts_with(const basic_string_view _View) const noexcept {
        // test if this instance begins with _View
        return _Mysize >= _View._Mysize && _Traits::compare(_Mydata, _View._Mydata, _View._Mysize) == 0;
    }

private:
    constexpr void _Check_offset(const size_type _Off) const { // checks whether _Off is in the bounds of [0, size()]
        if (_Mysize < _Off) {
            _Xran();
        }
    }

    constexpr void _Check_offset_exclusive(const size_type _Off) const {
        // checks whether _Off is in the bounds of [0, size())
        if (_Mysize <= _Off) {
            _Xran();
        }
    }

    constexpr size_type _Clamp_suffix_size(const size_type _Off, const size_type _Size) const noexcept {
        // trims _Size to the longest it can be assuming a string at/after _Off
        return _Min_value(_Size, _Mysize - _Off);
    }

    [[noreturn]] static void _Xran() { // report an out_of_range error
        _Xout_of_range("invalid string_view position");
    }

    const_pointer _Mydata;
    size_type _Mysize;
#endif

#if 0
  StringView(const char* text) : StringView{text, std::strlen(text)} {}

  StringView(const char* text, StringLength length)
    : m_text{static_cast<Char*>(const_cast<Char*>(text))}, m_length{length} {}

  StringView(const StringView& other, StringLength length)
    : m_text{other.m_text}, m_length{length} {}

  Char operator[](StringLength index) const {
    return m_text[index];
  }

  bool operator==(const StringView& other) const {
    return compare(other) == 0;
  }

  bool operator!=(const StringView& other) const {
    return compare(other) != 0;
  }

  bool operator<(const StringView& other) const {
    return compare(other) < 0;
  }

  Char* getData() const {
    return m_text;
  }

  StringLength getLength() const {
    return m_length;
  }

  bool isEmpty() const {
    return m_length == 0;
  }

  I32 compare(const StringView& other) const {
    if (m_length != other.m_length) {
      return static_cast<I32>(m_length - other.m_length);
    }

    return ::strncmp(m_text, other.m_text, std::min(m_length, other.m_length));
  }

  // Return a new StringView, starting from the startIndex and ending where this StringView ended.
  StringView subString(StringLength startIndex) const {
    return StringView{m_text + startIndex, m_length - startIndex};
  }

  // Get a new StringView for a part of string inside this one.
  StringView subString(StringLength startIndex, StringLength length) const {
    return StringView{m_text + startIndex,
                      (startIndex + length > m_length) ? m_length - startIndex : length};
  }

  // Return the position of the first character that matches the given character.
  StringLength findFirstOf(Char ch) const {
    for (StringLength i = 0; i < m_length; ++i) {
      if (ch == m_text[i]) {
        return i;
      }
    }

    return kInvalidPosition;
  }

  // Return the position of the first character that matches any of the predicate characters.
  StringLength findFirstOfAny(const StringView& characters) const {
    for (StringLength i = 0; i < m_length; ++i) {
      auto pos = characters.findFirstOf(m_text[i]);
      if (pos != kInvalidPosition) {
        return i;
      }
    }

    return kInvalidPosition;
  }

  // Return the position of the last character that matches any of the predicate characters.
  StringLength findLastOfAny(const StringView& characters) {
    for (StringLength i = 0; i < m_length; ++i) {
      auto currentPos = m_length - 1 - i;
      auto pos = characters.findFirstOf(m_text[currentPos]);
      if (pos != kInvalidPosition) {
        return currentPos;
      }
    }

    return kInvalidPosition;
  }

protected:
  template <MemSize Size>
  friend class StaticString;

  Char* m_text;
  StringLength m_length;
#endif

private:
  const Char* m_data;
  StringLength m_length;
};

inline OutputStream& operator<<(OutputStream& stream, const nu::StringView& value) {
  stream.write((char*)value.data(), value.length());
  return stream;
}

inline std::ostream& operator<<(std::ostream& os, const StringView& value) {
  for (StringLength i = 0; i < value.length(); ++i) {
    os << static_cast<I8>(value[i]);
  }
  return os;
}

}  // namespace nu

#endif  // NUCLEUS_TEXT_STRING_VIEW_H_
