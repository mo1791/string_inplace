#ifndef STRING_INPLACE_HXX
#define STRING_INPLACE_HXX

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>
#include <string_view>


template <typename CharT, typename Traits>
using basic_string_view = std::basic_string_view<CharT, Traits>;


namespace utils
{

    template <typename T> inline void throw_helper( const std::string &_msg )
    {
    #ifndef _NO_EXCEPTIONS
        throw T(_msg);
    #else
        std::cerr << _msg.c_str() << "\n";
        std::abort();
    #endif
    }


/** CONSTRAINTS **/

//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    template <typename It, typename ItUp>
    struct is_iterator_convertible_to
        : std::integral_constant<bool,
                                std::is_convertible<typename std::iterator_traits<It>::iterator_category, ItUp>::value>
    {
    };
//	----------------------------------------------------------------------------
    template <typename It, typename ItUp>
    concept _convertible_to =  is_iterator_convertible_to<It, ItUp>::value;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    template <typename It> 
    struct is_input_iterator : public is_iterator_convertible_to<It, std::input_iterator_tag>
    {
    };
//	----------------------------------------------------------------------------
    template <typename It>
    concept _input_iterator = is_input_iterator<It>::value;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    template <typename It>
    struct is_forward_iterator : public is_iterator_convertible_to<It, std::forward_iterator_tag>
    {
    };
//	----------------------------------------------------------------------------
    template <typename It>
    concept _forward_iterator = is_forward_iterator<It>::value;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    template <typename It>
    struct is_exactly_input_iterator
        : public std::integral_constant<bool, is_input_iterator<It>::value && !is_forward_iterator<It>::value>
    {
    };
//	----------------------------------------------------------------------------
    template <typename It>
    concept _exactly_input_iterator = is_exactly_input_iterator<It>::value;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------

/** END CONSTRAINTS **/


    struct is_exactly_input_iterator_tag {};

    struct is_input_iterator_tag {};




    template <typename CharT, typename Traits>
    const CharT *search_substring( const CharT *_first1, const CharT *_last1, const CharT *_first2, const CharT *_last2 );

} // END namespace utils

template <std::size_t N, typename CharT = char, typename Traits = std::char_traits<CharT>> 
class basic_inplace_string
{

public:
    using __self = basic_inplace_string;

    using traits_type             = Traits;
    using value_type              = CharT;
    using reference               = value_type &;
    using const_reference         = const value_type &;
    using pointer                 = value_type *;
    using const_pointer           = const value_type *;
    using size_type               = std::size_t;
    using difference_type         = std::ptrdiff_t;
    using iterator                = pointer;
    using const_iterator          = const_pointer;
    using reverse_iterator        = std::reverse_iterator<iterator>;
    using const_reverse_iterator  = std::reverse_iterator<const_iterator>;

private:
    using static_size_type = std::make_unsigned_t<value_type>;

public:
    static constexpr const size_type npos = static_cast<size_type>(-1);


/** CONSTRUCTORS **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    explicit basic_inplace_string() noexcept;
//	----------------------------------------------------------------------------
	template <std::size_t M>
	basic_inplace_string( const value_type (& _str)[M] ) noexcept;
//	----------------------------------------------------------------------------
	template < typename ValueTypePtr >
	basic_inplace_string( ValueTypePtr _ptr ) requires( std::same_as<ValueTypePtr, const value_type *> );
//	----------------------------------------------------------------------------
	basic_inplace_string( size_type _count , value_type _chr );
//	----------------------------------------------------------------------------
	basic_inplace_string( const std::basic_string<CharT, Traits> &_other, size_type _pos );
//	----------------------------------------------------------------------------
	basic_inplace_string( const basic_inplace_string &_other, size_type _pos );
//	----------------------------------------------------------------------------
	basic_inplace_string( const std::basic_string<CharT, Traits> &_other, size_type _pos, size_type _count );
//	----------------------------------------------------------------------------
	basic_inplace_string( const basic_inplace_string &_other, size_type _pos, size_type _count);
//	----------------------------------------------------------------------------
	basic_inplace_string( const value_type *_str, size_type _count );
//	----------------------------------------------------------------------------    
	template <typename InputIt>
    basic_inplace_string(InputIt _first, InputIt _last);
//	----------------------------------------------------------------------------    
	basic_inplace_string( std::initializer_list<CharT> );
//	----------------------------------------------------------------------------
	explicit basic_inplace_string(const std::basic_string<CharT, Traits> &_str);
//	----------------------------------------------------------------------------
	explicit basic_inplace_string(basic_string_view<CharT, Traits> _str_v);
//	----------------------------------------------------------------------------
	template <typename T>
    basic_inplace_string(const T &t, size_type pos, size_type n) requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/** ACCESS **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    reference at(size_type _index);
//	----------------------------------------------------------------------------
    const_reference at(size_type _index) const;
//	----------------------------------------------------------------------------
    reference operator[](size_type _index);
//	----------------------------------------------------------------------------
    const_reference operator[](size_type _index) const;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/** **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    reference front();
//	----------------------------------------------------------------------------

//	----------------------------------------------------------------------------
    const_reference front() const;
//	----------------------------------------------------------------------------

//	----------------------------------------------------------------------------
    reference back();
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    const_reference back() const;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    value_type *data() noexcept;
//	------------------------------------------------------------------
//	----------------------------------------------------------------------------
    const value_type *data() const noexcept;
//	---------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    const value_type *c_str() const noexcept;
//	--------------------------------------------------------------
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/** CONVERSION OP **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    operator basic_string_view<CharT, Traits>() const noexcept
    {
        return { m_data.data(), size() };
    }
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/** **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    iterator begin() noexcept;
//	----------------------------------------------------------------------------
    const_iterator begin() const noexcept;
//	----------------------------------------------------------------------------
    const_iterator cbegin() const noexcept;
//	----------------------------------------------------------------------------
    const_iterator end() const noexcept;
//	----------------------------------------------------------------------------
	const_iterator cend() const noexcept;
//	----------------------------------------------------------------------------
    reverse_iterator rbegin() noexcept;
//	----------------------------------------------------------------------------
    const_reverse_iterator rbegin() const noexcept;
//	----------------------------------------------------------------------------
    const_reverse_iterator crbegin() const noexcept;
//	----------------------------------------------------------------------------
    reverse_iterator rend() noexcept;
//	----------------------------------------------------------------------------
    const_reverse_iterator rend() const noexcept;
//	----------------------------------------------------------------------------
    const_reverse_iterator crend() const noexcept;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/** **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    bool empty() const noexcept { return get_remaining_size() == max_size(); }
//	----------------------------------------------------------------------------
    size_type size() const noexcept { return N - reinterpret_cast<const static_size_type &>(m_data[N]); }
//	----------------------------------------------------------------------------
    size_type length() const noexcept { return size(); }
//	----------------------------------------------------------------------------
    static constexpr size_type max_size()  noexcept { return N; }
//	----------------------------------------------------------------------------
    static constexpr size_type capacity() noexcept { return N; }
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void shrink_to_fit() noexcept;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------

/** **/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void clear() noexcept
    {
        *this = __self{};
    }
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    basic_inplace_string &insert( size_type _index, size_type _count, value_type _chr );
//	----------------------------------------------------------------------------
	basic_inplace_string &insert( size_type _index, const value_type *_str );
//	----------------------------------------------------------------------------
	basic_inplace_string &insert( size_type  index, const value_type *_str, size_type _count );
//	----------------------------------------------------------------------------
	basic_inplace_string &insert( size_type  index, const basic_inplace_string &_str );
//	----------------------------------------------------------------------------	
	basic_inplace_string &insert( size_type  index, const basic_inplace_string &_str, size_type _index_str,
                                 size_type _count = npos );
//	----------------------------------------------------------------------------
    iterator insert( const_iterator _pos, value_type _chr );
//	----------------------------------------------------------------------------
	iterator insert( const_iterator _pos, size_type _count, value_type _chr );
//	----------------------------------------------------------------------------
    template <typename InputIt> 
	iterator insert( const_iterator _pos, InputIt _first, InputIt _last );
//	----------------------------------------------------------------------------
    iterator insert( const_iterator _pos, std::initializer_list<CharT> _ilist );
//	----------------------------------------------------------------------------
    basic_inplace_string &insert( size_type _pos, basic_string_view<CharT, Traits> _view );
//	----------------------------------------------------------------------------
    template <typename T>
	requires ( std::convertible_to<const T &, basic_string_view<CharT, Traits>> && !std::convertible_to<const T &, const CharT *> )
    basic_inplace_string &insert( size_type _pos, const T &_t, size_type _index_str, size_type _count = npos );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
	basic_inplace_string &erase( size_type _pos = 0ul, size_type _count = npos );
//	----------------------------------------------------------------------------
	iterator erase( const_iterator _pos );
//	----------------------------------------------------------------------------
	iterator erase( const_iterator _first, const_iterator _last );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------



/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void push_back( value_type _chr );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void pop_back();
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    basic_inplace_string &append( size_type _count, value_type _ch );
//	----------------------------------------------------------------------------
    basic_inplace_string &append( const std::basic_string<CharT, Traits> &_str );
//	----------------------------------------------------------------------------
    basic_inplace_string &append( const std::basic_string<CharT, Traits> &_str, size_type _pos, size_type _count = npos );
//	----------------------------------------------------------------------------    
	basic_inplace_string &append( const value_type *_str, size_type _count );
//	----------------------------------------------------------------------------
    basic_inplace_string &append( const value_type *_str );
//	----------------------------------------------------------------------------
    template <typename InputIt> basic_inplace_string &append( InputIt _first, InputIt _last );
//	----------------------------------------------------------------------------
    basic_inplace_string &append( std::initializer_list<value_type> _ilist );
//	----------------------------------------------------------------------------
    basic_inplace_string &append( const basic_string_view<CharT, Traits> &_view );
//	----------------------------------------------------------------------------
    template <typename T>
	requires ( std::convertible_to<const T &, basic_string_view<CharT, Traits>> && !std::convertible_to<const T &, const CharT *> )
    basic_inplace_string &append( const T &_t, size_type _pos, size_type _count = npos );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    basic_inplace_string &operator+=( const std::basic_string<CharT, Traits> &_str )
    {
        return append(_str);
    }
//	----------------------------------------------------------------------------
    basic_inplace_string &operator+=( value_type _chr )
    {
        push_back(_chr);
        return *this;
    }
//	----------------------------------------------------------------------------
    basic_inplace_string &operator+=( const value_type *_str )
    {
        return append(_str);
    }
//	----------------------------------------------------------------------------
    basic_inplace_string &operator+=( std::initializer_list<value_type> _ilist )
    {
        return append(_ilist);
    }
//	----------------------------------------------------------------------------
    basic_inplace_string &operator+=( basic_string_view<CharT, Traits> _view )
    {
        return append(_view);
    }
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    int compare( const basic_inplace_string &_str ) const noexcept;
//	----------------------------------------------------------------------------
    int compare( size_type _pos1, size_type _count1, const basic_inplace_string &_str ) const;
//	----------------------------------------------------------------------------
    int compare( size_type _pos1, size_type _count1, const basic_inplace_string &_str, size_type _pos2,
                size_type _count2 = npos ) const;
//	----------------------------------------------------------------------------
    int compare( const value_type *_str ) const;
//	----------------------------------------------------------------------------
    int compare( size_type pos1, size_type _count1, const value_type *_str ) const;
//	----------------------------------------------------------------------------
    int compare( size_type pos1, size_type _count1, const value_type *_str, size_type _count2 ) const;
//	----------------------------------------------------------------------------
    int compare( basic_string_view<CharT, Traits> __str_v ) const noexcept;
//	----------------------------------------------------------------------------
    int compare( size_type _pos1, size_type _count1, basic_string_view<CharT, Traits> __str_v ) const;
//	----------------------------------------------------------------------------
    template <typename T>
	requires ( std::convertible_to<const T &, basic_string_view<CharT, Traits>> && !std::convertible_to<const T &, const CharT *> )
    int compare( size_type _pos1, size_type _count1, const T &_t, size_type _pos2, size_type _count2 = npos ) const;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( size_type _pos, size_type _count, const basic_inplace_string &_str );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, const basic_inplace_string &_str );
//	----------------------------------------------------------------------------    
	basic_inplace_string &replace( size_type _pos, size_type _count, const basic_inplace_string &_str, size_type _pos2,
                                  size_type _count2 = npos );
//	----------------------------------------------------------------------------
    template <class InputIt>
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, InputIt _first2, InputIt _last2 );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( size_type _pos, size_type _count, const CharT *_str, size_type _count2 );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, const CharT *_str, size_type _count2 );
//	----------------------------------------------------------------------------    
	basic_inplace_string &replace( size_type _pos, size_type _count, const CharT *_str );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, const CharT *_str );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( size_type _pos, size_type _count, size_type _count2, value_type _chr );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, size_type _count2, value_type _chr );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, std::initializer_list<value_type> _ilist );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( size_type _pos, size_type _count, basic_string_view<CharT, Traits> __str_v );
//	----------------------------------------------------------------------------
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, basic_string_view<CharT, Traits> __str_v );
//	----------------------------------------------------------------------------
    template <typename T>
	requires ( std::convertible_to<const T &, basic_string_view<CharT, Traits>> && !std::convertible_to<const T &, const CharT *> )
    basic_inplace_string &replace( size_type _pos, size_type _count, const T &_t, size_type _pos2, size_type _count2 = npos);
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    basic_inplace_string substr( size_type _pos = 0ul, size_type _count = npos ) const;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    size_type copy( value_type *_dest, size_type _count, size_type _pos = 0ul ) const;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void resize( size_type _sz );
//	----------------------------------------------------------------------------
    void resize( size_type _new_size, value_type _chr );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void swap( basic_inplace_string &_other ) noexcept;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


/****/
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    size_type find( const basic_inplace_string &_other, size_type _pos = 0ul ) const noexcept;
//	----------------------------------------------------------------------------
    size_type find( const value_type *_str, size_type _pos, size_type _count ) const noexcept;
//	----------------------------------------------------------------------------    
	size_type find( const value_type *_str, size_type _pos = 0ul ) const noexcept;
//	----------------------------------------------------------------------------    
	size_type find( value_type _ch, size_type _pos = 0ul ) const noexcept;
//	----------------------------------------------------------------------------
    size_type find( basic_string_view<CharT, Traits> __str_v, size_type _pos = 0ul ) const noexcept;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------



private:
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------    
	template <typename InputIt>
    basic_inplace_string( InputIt _first, InputIt _last, utils::is_exactly_input_iterator_tag );
//	----------------------------------------------------------------------------
    template <typename InputIt>
	basic_inplace_string( InputIt _first, InputIt _last, utils::is_input_iterator_tag );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    template <typename InputIt>
    iterator insert( const_iterator _pos, InputIt _first, InputIt _last, utils::is_exactly_input_iterator_tag );
//	----------------------------------------------------------------------------
    template <typename InputIt>
    iterator insert( const_iterator _pos, InputIt _first, InputIt _last, utils::is_input_iterator_tag );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    template <typename InputIt>
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, InputIt _first2, InputIt _last2,
                                  utils::is_exactly_input_iterator_tag );
//	----------------------------------------------------------------------------
    template <typename InputIt>
    basic_inplace_string &replace( const_iterator _first, const_iterator _last, InputIt _first2, InputIt _last2,
                                  utils::is_input_iterator_tag );
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------


//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
    void set_size(size_type sz) noexcept;
//	----------------------------------------------------------------------------
	size_type get_remaining_size() const noexcept;
//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------

private:
    std::array<value_type, N + 1> m_data;
};

// 	*****************************
	#include "string_inplace.txx"
//	*****************************

/** OSTREAM OVERLOAD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto operator<<(std::basic_ostream<CharT, Traits> &_ostream,
												const basic_inplace_string<N, CharT, Traits> &_str)
		-> std::basic_ostream<CharT, Traits> &
	{
		return _ostream.write(_str.data(), static_cast<std::streamsize>(_str.size()));
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------




/** COMP OPERATOR OVERLOAD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, std::size_t M, typename CharT, typename Traits>
	inline bool operator==(const basic_inplace_string<N, CharT, Traits> &_lhs, const basic_inplace_string<M, CharT, Traits> &_rhs)
	{
		return ( _lhs.size() == _rhs.size() ) && ( Traits::compare(_lhs.data(), _rhs.data(), _lhs.size()) == 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator==(const basic_inplace_string<N, CharT, Traits> &_lhs, const CharT *_rhs)
	{
		assert( _rhs != nullptr );

		return ( _lhs.size() == Traits::length(_rhs) ) && ( Traits::compare(_lhs.data(), _rhs, _lhs.size()) == 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator==(const CharT *_lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs == _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator==(const basic_inplace_string<N, CharT, Traits> &_lhs, T _rhs)
	{
		basic_string_view<CharT, Traits> _str_v = _rhs;

		return ( _lhs.size() == _str_v.size() ) && ( Traits::compare(_lhs.data(), _str_v.data(), _lhs.size()) == 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator==(T _lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs == _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, std::size_t M, typename CharT, typename Traits>
	inline bool operator!=(const basic_inplace_string<N, CharT, Traits> &_lhs, const basic_inplace_string<M, CharT, Traits> &_rhs)
	{
		return !(_lhs == _rhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator!=(const basic_inplace_string<N, CharT, Traits> &_lhs, const CharT *_rhs)
	{
		assert( _rhs != nullptr );

		return ( _lhs.size() != Traits::length(_rhs) ) || ( Traits::compare(_lhs.data(), _rhs, _lhs.size()) != 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator!=(const CharT *_lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs != _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator!=(const basic_inplace_string<N, CharT, Traits> &_lhs, T _rhs)
	{
		return !(_lhs == _rhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator!=(T _lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs != _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, std::size_t M, typename CharT, typename Traits>
	inline bool operator<(const basic_inplace_string<N, CharT, Traits> &_lhs, const basic_inplace_string<M, CharT, Traits> &_rhs)
	{
		return ( _lhs.compare(_rhs) < 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator<(const basic_inplace_string<N, CharT, Traits> &_lhs, const CharT *_rhs)
	{
		return (_lhs.compare(_rhs) < 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator<(const CharT *_lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs.compare(_lhs) > 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator<(const basic_inplace_string<N, CharT, Traits> &_lhs, T _rhs)
	{
		basic_string_view<CharT, Traits> _view = _rhs;

		return ( _lhs.compare(_view) < 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator<(T _lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs.compare(_lhs) > 0ul );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, std::size_t M, typename CharT, typename Traits>
	inline bool operator>(const basic_inplace_string<N, CharT, Traits> &_lhs, const basic_inplace_string<M, CharT, Traits> &_rhs)
	{
		return ( _rhs < _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator>(const basic_inplace_string<N, CharT, Traits> &_lhs, const CharT *_rhs)
	{
		return ( _rhs < _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline bool operator>(const CharT *_lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs < _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator>(const basic_inplace_string<N, CharT, Traits> &_lhs, T _rhs)
	{
		return ( _rhs < _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline bool operator>(T _lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return ( _rhs < _lhs );
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, std::size_t M, typename CharT, typename Traits>
	inline inline bool operator<=(const basic_inplace_string<N, CharT, Traits> &_lhs, const basic_inplace_string<M, CharT, Traits> &_rhs)
	{
		return !(_rhs < _lhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline inline bool operator<=(const basic_inplace_string<N, CharT, Traits> &_lhs, const CharT *_rhs)
	{
		return !(_rhs < _lhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline inline bool operator<=(const CharT *_lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return !(_rhs < _lhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline inline bool operator<=(const basic_inplace_string<N, CharT, Traits> &_lhs, T _rhs)
	{
		return !(_rhs < _lhs);
	}
//	-----------------------------------------------------------------------------------------------
	template < std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline inline bool operator<=(T _lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return !(_rhs < _lhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, std::size_t M, typename CharT, typename Traits>
	inline inline bool operator>=(const basic_inplace_string<N, CharT, Traits> &_lhs, const basic_inplace_string<M, CharT, Traits> &_rhs)
	{
		return !(_lhs < _rhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline inline bool operator>=(const basic_inplace_string<N, CharT, Traits> &_lhs, const CharT *_rhs)
	{
		return !(_lhs < _rhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	inline inline bool operator>=(const CharT *_lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return !(_lhs < _rhs);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits, typename T>
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline inline bool operator>=(const basic_inplace_string<N, CharT, Traits> &_lhs, T _rhs)
	{
		return !(_lhs < _rhs);
	}
//	-----------------------------------------------------------------------------------------------
	template < std::size_t N, typename CharT, typename Traits, typename T >
		requires( std::convertible_to<const T &, basic_string_view<CharT, Traits>> )
	inline inline bool operator>=(T _lhs, const basic_inplace_string<N, CharT, Traits> &_rhs)
	{
		return !(_lhs < _rhs);
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------


//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	namespace utils
	{
	//	-----------------------------------------------------------------------------------------------
	//	-----------------------------------------------------------------------------------------------
		template <typename CharT, typename Traits>
		auto search_substring(const CharT *_first1, const CharT *_last1, const CharT *_first2, const CharT *_last2)
			-> const CharT *
		{
			assert( _last1 >= _first1 );
			assert( _last2 >= _first2 );

			const std::size_t _size2 = static_cast<std::size_t>(_last2 - _first2);

			while (true)
			{
				const std::size_t _size1 = static_cast<std::size_t>(_last1 - _first1);
				
				if ( _size1 < _size2 )
				{	
					return nullptr;
				}

				_first1 = Traits::find(_first1, _size1, *_first2);
				
				if ( _first1 == nullptr )
				{
					return nullptr;
				}

				if ( Traits::compare(_first1, _first2, _size2) == 0ul )
				{
					return _first1;
				}

				++_first1;
			}

			return nullptr;
		}
	//	-----------------------------------------------------------------------------------------------
	//	-----------------------------------------------------------------------------------------------

	} // namespace utils
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------


//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N> using inplace_string     = basic_inplace_string<N, char>;
	template <std::size_t N> using inplace_wstring    = basic_inplace_string<N, wchar_t>;
	template <std::size_t N> using inplace_u16string  = basic_inplace_string<N, char16_t>;
	template <std::size_t N> using inplace_u32string  = basic_inplace_string<N, char32_t>;
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------


//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	namespace std
	{
	//	-----------------------------------------------------------------------------------------------
	//	-----------------------------------------------------------------------------------------------
		template <std::size_t N, typename CharT, typename Traits>
			struct hash<basic_inplace_string<N, CharT, Traits>>
		{
			size_t operator()(const basic_inplace_string<N, CharT, Traits> &_str) const
			{
				using view_t = basic_string_view<CharT, Traits>;

				const view_t _view(_str.data(), _str.size());
				return std::hash<view_t>()(_view);
			}
		};
	//	-----------------------------------------------------------------------------------------------
	//	-----------------------------------------------------------------------------------------------
	} // namespace std
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

// END


#endif
