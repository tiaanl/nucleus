
#ifndef NUCLEUS_TEXT_STRING_VIEW_H_
#define NUCLEUS_TEXT_STRING_VIEW_H_

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Types.h"

#include <algorithm>
#include <cstring>
#include <ostream>

namespace nu {

#if 0

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
  static StringLength npos;

  using SizeType = StringLength;
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

  constexpr auto length() const noexcept -> SizeType {
    return m_length;
  }

  constexpr auto empty() const noexcept -> bool {
    return m_length == 0;
  }

  constexpr auto data() const noexcept -> const Char* {
    return m_data;
  }

  constexpr auto operator[](SizeType index) const noexcept -> const Char& {
    DCHECK(index < m_length);

    return m_data[index];
  }

  constexpr auto at(SizeType index) const -> const Char& {
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
#endif

  constexpr auto subString(SizeType offset = 0, SizeType length = npos) const -> StringView {
    DCHECK(offset < m_length) << "Index out of range.";
    length = std::min(length, m_length - offset);
    return StringView{m_data + offset, length};
  }

#if 0
    constexpr bool _Equal(const basic_string_view _Right) const noexcept { // test if *this and _Right are equal
        return _Traits_equal<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
    }
#endif

  constexpr auto compare(StringView right) const noexcept -> I32 {
    SizeType length = std::min(m_length, right.length());

    I32 result = 0;
    for (SizeType i = length; i; --i) {
      if (m_data[i] < right.m_data[i]) {
        result = -1;
        break;
      }
      if (right.m_data[i] < m_data[i]) {
        result = 1;
        break;
      }
    }

    if (result == 0) {
      result = m_length == right.m_length ? 0 : (m_length < right.m_length ? -1 : 1);
    }

    return result;
  }

#if 0
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
#endif

  constexpr auto findFirstOf(StringView s, SizeType start = 0) const noexcept -> SizeType {
    for (SizeType i = 0; i < m_length; ++i) {
      auto pos = s.findFirstOf(m_data[i]);
      if (pos != npos) {
        return i;
      }
    }

    return npos;
  }

  //  _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
  //  size_type find_first_of(basic_string_view __s, size_type __pos = 0) const _NOEXCEPT
  //  {
  //    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find_first_of():
  //    received nullptr"); return __str_find_first_of<value_type, size_type, traits_type, npos>
  //        (data(), size(), __s.data(), __pos, __s.size());
  //  }

  //  for (StringLength i = 0; i < m_length; ++i) {
  //    auto pos = characters.findFirstOf(m_text[i]);
  //    if (pos != kInvalidPosition) {
  //      return i;
  //    }
  //  }
  //
  //  return kInvalidPosition;

#if 0

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

constexpr auto operator==(StringView left, StringView right) noexcept {
  if (left.length() != right.length()) {
    return false;
  }
  return left.compare(right) == 0;
}

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

#endif

class BasicStringView {
public:
  // types
  typedef Char value_type;
  typedef Char* pointer;
  typedef const Char* const_pointer;
  typedef Char& reference;
  typedef const Char& const_reference;
  typedef const_pointer const_iterator;  // See [string.view.iterators]
  typedef const_iterator iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef const_reverse_iterator reverse_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  static constexpr const size_type npos = -1;  // size_type(-1);

  static_assert((!std::is_array<value_type>::value),
                "Character type of BasicStringView must not be an array");
  static_assert((std::is_standard_layout<value_type>::value),
                "Character type of BasicStringView must be standard-layout");
  static_assert((std::is_trivial<value_type>::value),
                "Character type of BasicStringView must be trivial");
  // static_assert((is_same<Char, typename traits_type::char_type>::value), "traits_type::char_type
  // must be the same type as CharT");

  // [string.view.cons], construct/copy
  constexpr BasicStringView() noexcept : m_data(nullptr), m_size(0) {}

  constexpr BasicStringView(const BasicStringView&) noexcept = default;

  constexpr BasicStringView& operator=(const BasicStringView&) noexcept = default;

  constexpr BasicStringView(const Char* __s, size_type __len) noexcept
    : m_data(__s), m_size(__len) {
    DCHECK(__len == 0 || __s != nullptr)
        << "string_view::string_view(Char *, size_t): received nullptr";
  }

  constexpr BasicStringView(const Char* __s)
    : m_data(__s), m_size(std::__char_traits_length_checked<_Traits>(__s)) {}

  // [string.view.iterators], iterators
  constexpr const_iterator begin() const noexcept {
    return cbegin();
  }

  constexpr const_iterator end() const noexcept {
    return cend();
  }

  constexpr const_iterator cbegin() const noexcept {
    return m_data;
  }

  constexpr const_iterator cend() const noexcept {
    return m_data + m_size;
  }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(cend());
  }

  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  // [string.view.capacity], capacity
  constexpr size_type size() const noexcept {
    return m_size;
  }

  constexpr size_type length() const noexcept {
    return m_size;
  }

  constexpr size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
  }

  NODISCARD constexpr bool empty() const noexcept {
    return m_size == 0;
  }

  // [string.view.access], element access
  constexpr const_reference operator[](size_type __pos) const noexcept {
    return m_data[__pos];
  }

  constexpr const_reference at(size_type __pos) const {
    return __pos >= size() ? (__throw_out_of_range("string_view::at"), m_data[0]) : m_data[__pos];
  }

  constexpr const_reference front() const noexcept {
    return DCHECK(!empty()); // << "string_view::front(): string is empty";  // << m_data[0];
  }

  constexpr const_reference back() const noexcept {
    DCHECK(!empty()); // << "string_view::back(): string is empty"), m_data[m_size - 1];
  }

  constexpr const_pointer data() const noexcept {
    return m_data;
  }

  // [string.view.modifiers], modifiers:
  constexpr void remove_prefix(size_type __n) noexcept {
    DCHECK(__n <= size());//, "remove_prefix() can't remove more than size()");
    m_data += __n;
    m_size -= __n;
  }

  constexpr void remove_suffix(size_type __n) noexcept {
    DCHECK(__n <= size());//, "remove_suffix() can't remove more than size()");
    m_size -= __n;
  }

  constexpr void swap(BasicStringView& __other) noexcept {
    const value_type* __p = m_data;
    m_data = __other.m_data;
    __other.m_data = __p;

    size_type __sz = m_size;
    m_size = __other.m_size;
    __other.m_size = __sz;
  }

  size_type copy(Char* __s, size_type __n, size_type __pos = 0) const {
    if (__pos > size())
      __throw_out_of_range("string_view::copy");
    size_type __rlen = _VSTD::min(__n, size() - __pos);
    _Traits::copy(__s, data() + __pos, __rlen);
    return __rlen;
  }

  constexpr BasicStringView substr(size_type __pos = 0, size_type __n = npos) const {
    return __pos > size() ? (__throw_out_of_range("string_view::substr"), BasicStringView())
                          : BasicStringView(data() + __pos, _VSTD::min(__n, size() - __pos));
  }

  constexpr int compare(BasicStringView __sv) const noexcept {
    size_type __rlen = _VSTD::min(size(), __sv.size());
    int __retval = char_traits<Char>::compare(data(), __sv.data(), __rlen);
    if (__retval == 0)  // first __rlen chars matched
      __retval = size() == __sv.size() ? 0 : (size() < __sv.size() ? -1 : 1);
    return __retval;
  }

  constexpr int compare(size_type __pos1, size_type __n1, BasicStringView __sv) const {
    return substr(__pos1, __n1).compare(__sv);
  }

  constexpr int compare(size_type __pos1, size_type __n1, BasicStringView __sv, size_type __pos2,
                        size_type __n2) const {
    return substr(__pos1, __n1).compare(__sv.substr(__pos2, __n2));
  }

  constexpr int compare(const Char* __s) const noexcept {
    return compare(BasicStringView(__s));
  }

  constexpr int compare(size_type __pos1, size_type __n1, const Char* __s) const {
    return substr(__pos1, __n1).compare(BasicStringView(__s));
  }

  constexpr int compare(size_type __pos1, size_type __n1, const Char* __s, size_type __n2) const {
    return substr(__pos1, __n1).compare(BasicStringView(__s, __n2));
  }

  // find
  constexpr size_type find(BasicStringView __s, size_type __pos = 0) const noexcept {
    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr,
                   "string_view::find(): received nullptr");
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __s.data(), __pos,
                                                                __s.size());
  }

  constexpr size_type find(Char __c, size_type __pos = 0) const noexcept {
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __c, __pos);
  }

  constexpr size_type find(const Char* __s, size_type __pos, size_type __n) const {
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find(): received nullptr");
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
  }

  constexpr size_type find(const Char* __s, size_type __pos = 0) const {
    _LIBCPP_ASSERT(__s != nullptr, "string_view::find(): received nullptr");
    return __str_find<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos,
                                                                traits_type::length(__s));
  }

  // rfind
  constexpr size_type rfind(BasicStringView __s, size_type __pos = npos) const noexcept {
    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr,
                   "string_view::find(): received nullptr");
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __s.data(), __pos,
                                                                 __s.size());
  }

  constexpr size_type rfind(Char __c, size_type __pos = npos) const noexcept {
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __c, __pos);
  }

  constexpr size_type rfind(const Char* __s, size_type __pos, size_type __n) const {
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::rfind(): received nullptr");
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos, __n);
  }

  constexpr size_type rfind(const Char* __s, size_type __pos = npos) const {
    _LIBCPP_ASSERT(__s != nullptr, "string_view::rfind(): received nullptr");
    return __str_rfind<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos,
                                                                 traits_type::length(__s));
  }

  // find_first_of
  constexpr size_type find_first_of(BasicStringView __s, size_type __pos = 0) const noexcept {
    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr,
                   "string_view::find_first_of(): received nullptr");
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __s.data(),
                                                                         __pos, __s.size());
  }

  constexpr size_type find_first_of(Char __c, size_type __pos = 0) const noexcept {
    return find(__c, __pos);
  }

  constexpr size_type find_first_of(const Char* __s, size_type __pos, size_type __n) const {
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_first_of(): received nullptr");
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos,
                                                                         __n);
  }

  constexpr size_type find_first_of(const Char* __s, size_type __pos = 0) const {
    _LIBCPP_ASSERT(__s != nullptr, "string_view::find_first_of(): received nullptr");
    return __str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos,
                                                                         traits_type::length(__s));
  }

  // find_last_of
  constexpr size_type find_last_of(BasicStringView __s, size_type __pos = npos) const noexcept {
    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr,
                   "string_view::find_last_of(): received nullptr");
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __s.data(),
                                                                        __pos, __s.size());
  }

  constexpr size_type find_last_of(Char __c, size_type __pos = npos) const noexcept {
    return rfind(__c, __pos);
  }

  constexpr size_type find_last_of(const Char* __s, size_type __pos, size_type __n) const {
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_last_of(): received nullptr");
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos,
                                                                        __n);
  }

  constexpr size_type find_last_of(const Char* __s, size_type __pos = npos) const {
    _LIBCPP_ASSERT(__s != nullptr, "string_view::find_last_of(): received nullptr");
    return __str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), __s, __pos,
                                                                        traits_type::length(__s));
  }

  // find_first_not_of
  constexpr size_type find_first_not_of(BasicStringView __s, size_type __pos = 0) const noexcept {
    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr,
                   "string_view::find_first_not_of(): received nullptr");
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(
        data(), size(), __s.data(), __pos, __s.size());
  }

  constexpr size_type find_first_not_of(Char __c, size_type __pos = 0) const noexcept {
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __c,
                                                                             __pos);
  }

  constexpr size_type find_first_not_of(const Char* __s, size_type __pos, size_type __n) const {
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr,
                   "string_view::find_first_not_of(): received nullptr");
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), __s,
                                                                             __pos, __n);
  }

  constexpr size_type find_first_not_of(const Char* __s, size_type __pos = 0) const {
    _LIBCPP_ASSERT(__s != nullptr, "string_view::find_first_not_of(): received nullptr");
    return __str_find_first_not_of<value_type, size_type, traits_type, npos>(
        data(), size(), __s, __pos, traits_type::length(__s));
  }

  // find_last_not_of
  constexpr size_type find_last_not_of(BasicStringView __s, size_type __pos = npos) const noexcept {
    _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr,
                   "string_view::find_last_not_of(): received nullptr");
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(
        data(), size(), __s.data(), __pos, __s.size());
  }

  constexpr size_type find_last_not_of(Char __c, size_type __pos = npos) const noexcept {
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __c,
                                                                            __pos);
  }

  constexpr size_type find_last_not_of(const Char* __s, size_type __pos, size_type __n) const {
    _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_last_not_of(): received nullptr");
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), __s,
                                                                            __pos, __n);
  }

  constexpr size_type find_last_not_of(const Char* __s, size_type __pos = npos) const {
    _LIBCPP_ASSERT(__s != nullptr, "string_view::find_last_not_of(): received nullptr");
    return __str_find_last_not_of<value_type, size_type, traits_type, npos>(
        data(), size(), __s, __pos, traits_type::length(__s));
  }

  constexpr bool starts_with(BasicStringView __s) const noexcept {
    return size() >= __s.size() && compare(0, __s.size(), __s) == 0;
  }

  constexpr bool starts_with(value_type __c) const noexcept {
    return !empty() && _Traits::eq(front(), __c);
  }

  constexpr bool starts_with(const value_type* __s) const noexcept {
    return starts_with(BasicStringView(__s));
  }

  constexpr bool ends_with(BasicStringView __s) const noexcept {
    return size() >= __s.size() && compare(size() - __s.size(), npos, __s) == 0;
  }

  constexpr bool ends_with(value_type __c) const noexcept {
    return !empty() && _Traits::eq(back(), __c);
  }

  constexpr bool ends_with(const value_type* __s) const noexcept {
    return ends_with(BasicStringView(__s));
  }

private:
  const value_type* m_data;
  size_type m_size;
};

using StringView = BasicStringView;

}  // namespace nu

#endif  // NUCLEUS_TEXT_STRING_VIEW_H_
