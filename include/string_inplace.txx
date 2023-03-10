/** CTORS **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string() noexcept
	{
		m_data[0ul] = value_type{};
		set_size(0ul);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <std::size_t M>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const value_type (&_str)[M]) noexcept
	{
		constexpr size_type _size = M - 1ul;
		static_assert(_size <= max_size(), "basic_inplace_string: size exceeds maximum capacity");

		std::for_each_n(std::begin(_str), _size,
						[&, _index = 0ul](CharT _chr) mutable { traits_type::assign(m_data[_index++], _chr); });

		traits_type::assign(m_data[_size], value_type{});
		set_size(_size);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename ValueTypePtr>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(ValueTypePtr _str)
		requires(std::same_as<ValueTypePtr, const_pointer>)
		: basic_inplace_string(_str, traits_type::length(_str))
	{
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(size_type _count, value_type _chr)
	{
		set_size(0ul);
		insert(0ul, _count, _chr);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::basic_string<CharT, Traits> &_other,
																size_type _pos)
	{
		if (_pos > _other.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string: out of range");
		}

		set_size(0ul);
		insert(0ul, _other, _pos);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const basic_inplace_string &_other, size_type _pos)
	{
		if (_pos > _other.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string: out of range");
		}

		set_size(0ul);
		insert(0ul, _other, _pos);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::basic_string<CharT, Traits> &_other,
																size_type _pos, size_type _count)
	{
		if (_pos > _other.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string: out of range");
		}

		set_size(0ul);
		insert(0ul, _other, _pos, _count);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const basic_inplace_string &_other, size_type _pos,
																size_type _count)
	{
		if (_pos > _other.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string: out of range");
		}

		set_size(0ul);
		insert(0ul, _other, _pos, _count);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const_pointer _str, size_type _count)
	{
		set_size(0ul);
		insert(0ul, _str, _count);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const std::basic_string<CharT, Traits> &_str)
		: basic_inplace_string(_str.data(), _str.size())
	{
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(std::initializer_list<CharT> _ilits)
		: basic_inplace_string(_ilits.begin(), _ilits.size())
	{
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(basic_string_view<CharT, Traits> _str_v)
		: basic_inplace_string(_str_v.data(), _str_v.size())
	{
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename T>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(const T &t, size_type _pos, size_type n)
		requires(std::convertible_to<const T &, basic_string_view<CharT, Traits>>)
	{
		set_size(0ul);

		basic_string_view<CharT, Traits> _str_v = t;
		_str_v = _str_v.substr(_pos, n);
		insert(0ul, _str_v.data(), _str_v.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(InputIt _first, InputIt _last)
		: basic_inplace_string(
			_first, _last,
			typename std::conditional<utils::is_exactly_input_iterator<InputIt>::value,
										utils::is_exactly_input_iterator_tag, utils::is_input_iterator_tag>::type{})
	{
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(InputIt _first, InputIt _last,
																utils::is_exactly_input_iterator_tag _tag)
	{
		set_size(0ul);
		insert(cbegin(), _first, _last, _tag);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	basic_inplace_string<N, CharT, Traits>::basic_inplace_string(InputIt _first, InputIt _last,
																utils::is_input_iterator_tag _tag)
	{
		set_size(0ul);
		insert(cbegin(), _first, _last, _tag);
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/** ACCESS METHD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::operator[](size_type _index) -> reference
	{
		assert(_index <= size());

		return m_data[_index];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::operator[](size_type _index) const -> const_reference
	{
		assert(_index <= size());

		return m_data[_index];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::at(size_type _index) -> reference
	{
		if (_index >= size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::at: out of range");
		}

		return m_data[_index];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::at(size_type _index) const -> const_reference
	{
		if (_index >= size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::at: out of range");
		}

		return m_data[_index];
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/** INSERT METHD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _index, size_type _count, value_type _chr)
		-> basic_inplace_string &
	{
		const size_type _size = size();

		if (_index > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");
		}

		if ((_size + _count) > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::insert: maximum capacity reached");
		}

		traits_type::move(m_data.data() + _index + _count, m_data.data() + _index, _size - _index);
		traits_type::assign(&m_data[_index], _count, _chr);

		const size_type _new_size = _size + _count;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _index, const_pointer _str) -> basic_inplace_string &
	{
		return insert(_index, _str, traits_type::length(_str));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _index, const_pointer _str, size_type _count)
		-> basic_inplace_string &
	{
		const size_type _size = size();

		if (_index > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");
		}

		if ((size() + _count) > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::insert: maximum capacity reached");
		}

		traits_type::move(m_data.data() + _index + _count, m_data.data() + _index, _size - _index);

		std::for_each_n(_str, _size,
						[&, _i = 0ul](CharT _chr) mutable { traits_type::assign(m_data[_index + _i++], _chr); });

		const size_type _new_size = _size + _count;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _index, const basic_inplace_string &_str)
		-> basic_inplace_string &
	{
		return insert(_index, _str.data(), _str.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _index, const basic_inplace_string &_str,
														size_type _index_str, size_type _count) -> basic_inplace_string &
	{
		if (_index_str > _str.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");
		}

		basic_inplace_string _subs = _str.substr(_index_str, _count);

		return insert(_index, _subs.data(), _subs.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(const_iterator _pos, value_type _chr) ->
		typename basic_inplace_string::iterator
	{
		const size_type _index = static_cast<size_type>(_pos - m_data.data());

		insert(_index, 1ul, _chr);

		return (m_data.data() + _index);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(const_iterator _pos, size_type _count, value_type _chr) ->
		typename basic_inplace_string::iterator
	{
		const size_type _index = static_cast<size_type>(_pos - m_data.data());

		insert(_index, _count, _chr);

		return (m_data.data() + _index);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	auto basic_inplace_string<N, CharT, Traits>::insert(const_iterator _pos, InputIt _first, InputIt _last) ->
		typename basic_inplace_string::iterator
	{
		return insert(
			_pos, _first, _last,
			typename std::conditional<utils::is_exactly_input_iterator<InputIt>::value,
									utils::is_exactly_input_iterator_tag, utils::is_input_iterator_tag>::type{});
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	auto basic_inplace_string<N, CharT, Traits>::insert(const_iterator _pos, InputIt _first, InputIt _last,
														utils::is_exactly_input_iterator_tag) ->
		typename basic_inplace_string::iterator
	{
		assert((_pos >= m_data.data()) && (_pos <= m_data.data() + size()));

		const size_type _size  = size();
		const size_type _index = static_cast<size_type>(_pos - m_data.data());
		size_type _count       = 0ul;

		std::for_each(_first, _last, [&](CharT _chr) {
			traits_type::move(&m_data[_index + _count + 1ul], &m_data[_index + _count], _size - _index + _count);
			traits_type::assign(m_data[_index + _count], _chr);
		});

		const size_type _new_size = _size + _count;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return (m_data.data() + _index);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	auto basic_inplace_string<N, CharT, Traits>::insert(const_iterator _pos, InputIt _first, InputIt _last,
														utils::is_input_iterator_tag) ->
		typename basic_inplace_string::iterator
	{
		assert((_pos >= m_data.data()) && (_pos <= m_data.data() + size()));

		const size_type _size  = size();
		const size_type _index = static_cast<size_type>(_pos - m_data.data());
		const size_type _count = static_cast<size_type>(std::distance(_first, _last));

		traits_type::move(&m_data[_index + _count], &m_data[_index], _size - _index);

		std::for_each_n(_first, _count, [&, _i=0ul](CharT _chr) mutable {
			traits_type::assign(m_data[_index + _i++], _chr);
		});
		
		const size_type _new_size = _size + _count;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return (m_data.data() + _index);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(const_iterator _pos, std::initializer_list<CharT> _ilits) ->
		typename basic_inplace_string::iterator
	{
		assert((_pos >= m_data.data()) && (_pos <= m_data.data() + size()));

		const size_type _index = static_cast<size_type>(_pos - m_data.data());

		insert(_index, _ilits.begin(), _ilits.size());

		return (m_data.data() + _index);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _pos, basic_string_view<CharT, Traits> _view)
		-> basic_inplace_string &
	{
		return insert(_pos, _view.data(), _view.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename T>
		requires(std::convertible_to<const T &, basic_string_view<CharT, Traits>> &&
				!std::convertible_to<const T &, const CharT *>)
	auto basic_inplace_string<N, CharT, Traits>::insert(size_type _pos, const T &t, size_type _index_str, size_type _count)
		-> basic_inplace_string &
	{
		basic_string_view<CharT, Traits> _view = t;

		if (_index_str > _view.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::insert: out of range");
		}

		return insert(_pos, _view.data(), _index_str, std::min(_count, _view.size() - _index_str));
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/** ERASE METHD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::erase(size_type _index, size_type _count) -> basic_inplace_string &
	{
		size_type _size = size();
		_count = std::min(_size - _index, _count);

		if (_index > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::erase: out of range");
		}

		traits_type::move(m_data.data() + _index, m_data.data() + _index + _count, _size - _index - _count);

		_size -= _count;

		traits_type::assign(m_data[_size], value_type{});
		set_size(_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::erase(const_iterator position) -> typename basic_inplace_string::iterator
	{
		size_type _index = static_cast<size_type>(position - m_data.data());

		erase(_index, 1);

		return iterator{m_data.data() + _index};
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::erase(const_iterator _first, const_iterator _last) ->
		typename basic_inplace_string::iterator
	{
		const size_type _index = static_cast<size_type>(_first - m_data.data());
		const size_type _count = static_cast<size_type>(std::distance(_first, _last));

		erase(_index, _count);

		return iterator{m_data.data() + _index};
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/** APPEND METHD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(size_type _count, value_type ch) -> basic_inplace_string &
	{
		const size_type _size = size();
		if (_size + _count > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::append: exceed maximum string length");
		}

		traits_type::assign(m_data.data() + _size, _count, ch);

		const size_type _new_size = _size + _count;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(const std::basic_string<CharT, Traits> &_str)
		-> basic_inplace_string &
	{
		return append(_str.data(), _str.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(const std::basic_string<CharT, Traits> &_str, size_type _pos,
														size_type _count) -> basic_inplace_string &
	{
		return append(_str.data() + _pos, std::min(_str.size() - _pos, _count));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(const value_type *_str, size_type _count) -> basic_inplace_string &
	{
		const size_type _size = size();

		if ((_size + _count) > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::append: exceed maximum string length");
		}

		std::for_each_n(_str, _count, [&,_i=0ul](CharT _chr) mutable
		{
			traits_type::assign(m_data[_size + _i++], _chr);
		});
		

		const size_type _new_size = _size + _count;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(const_pointer _str) -> basic_inplace_string &
	{
		size_type _size = traits_type::length(_str);

		return append(_str, _size);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename InputIt>
	auto basic_inplace_string<N, CharT, Traits>::append(InputIt _first, InputIt _last) -> basic_inplace_string &
	{
		const size_type _size  = size();
		const size_type _count = std::distance(_first, _last);

		if ((_size + _count) > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::append: exceed maximum string length");
		}

		const pointer _ptr = m_data.data() + _size;
		
		std::for_each(_first, _last, [&](CharT _chr) mutable
		{
			traits_type::assign(*_ptr++, _chr);
		});
		
		traits_type::assign(*_ptr, value_type{});
		set_size(_size + _count);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(std::initializer_list<CharT> _ilits) -> basic_inplace_string &
	{
		return append(_ilits.begin(), _ilits.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::append(const basic_string_view<CharT, Traits> &_view)
		-> basic_inplace_string &
	{
		return append(_view.data(), _view.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename T>
		requires(std::convertible_to<const T &, basic_string_view<CharT, Traits>> &&
				!std::convertible_to<const T &, const CharT *>)
	auto basic_inplace_string<N, CharT, Traits>::append(const T &t, size_type _pos, size_type _count)
		-> basic_inplace_string &
	{
		basic_string_view<CharT, Traits> _view = t;

		return append((_view.data() + _pos), std::min((_view.size() - _pos), _count));
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(const basic_inplace_string &_str) const noexcept
	{
		return compare(0ul, size(), _str.data(), _str.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(size_type _pos1, size_type _count1,
														const basic_inplace_string &_str) const
	{
		return compare(_pos1, _count1, _str.data(), _str.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(size_type _pos1, size_type _count1,
														const basic_inplace_string &_str, size_type _pos2,
														size_type _count2) const
	{
		return compare(_pos1, _count1, _str.data() + _pos2, std::min(size() - _pos2, _count2));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(const_pointer _str) const
	{
		return compare(0ul, size(), _str, traits_type::length(_str));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(size_type _pos1, size_type _count1, const_pointer _str) const
	{
		return compare(_pos1, _count1, _str, traits_type::length(_str));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(size_type _pos1, size_type _count1, const_pointer _str,
														size_type _count2) const
	{
		const size_type _size = std::min(_count1, _count2);
		const int _cmp = traits_type::compare(data() + _pos1, _str, _size);

		if (_cmp != 0)
		{
			return _cmp;
		}

		return (_count1 > _count2) ? 1 : ((_count1 == _count2) ? 0 : -1);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(basic_string_view<CharT, Traits> _str_v) const noexcept
	{
		return compare(0ul, size(), _str_v.data(), _str_v.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	int basic_inplace_string<N, CharT, Traits>::compare(size_type _pos1, size_type _count1,
														basic_string_view<CharT, Traits> _str_v) const
	{
		return compare(_pos1, _count1, _str_v.data(), _str_v.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename T>
		requires(std::convertible_to<const T &, basic_string_view<CharT, Traits>> &&
				!std::convertible_to<const T &, const CharT *>)
	int basic_inplace_string<N, CharT, Traits>::compare(size_type _pos1, size_type _count1, const T &t, size_type _pos2,
														size_type _count2) const
	{
		basic_string_view<CharT, Traits> _view = t;

		if (_pos2 > _view.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		return compare(_pos1, _count1, _view.data() + _pos2, std::min(_view.size() - _pos2, _count2));
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos, size_type _count, const basic_inplace_string &_str)
		-> basic_inplace_string &
	{
		return replace(_pos, _count, _str.c_str(), _str.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last,
														const basic_inplace_string &_str) -> basic_inplace_string &
	{
		return replace((_first - m_data.data()), std::distance(_first, _last), _str.c_str(), _str.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos, size_type _count, const basic_inplace_string &_str,
														size_type _pos2, size_type _count2) -> basic_inplace_string &
	{
		if (_pos2 > _str.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		return replace(_pos, _count, _str.c_str() + _pos2, std::min(_str.size() - _pos2, _count2));
	}
//	------------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <class InputIt>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last, InputIt _first2,
														InputIt _last2) -> basic_inplace_string &
	{
		return replace(
			_first, _last, _first2, _last2,
			typename std::conditional<utils::is_exactly_input_iterator<InputIt>::value,
									utils::is_exactly_input_iterator_tag, utils::is_input_iterator_tag>::type{});
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <class InputIt>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last, InputIt _first2,
														InputIt _last2, utils::is_exactly_input_iterator_tag)
		-> basic_inplace_string &
	{
		const size_type _pos1    = static_cast<size_type>(_first - m_data.data());
		const size_type _count1  = static_cast<size_type>(std::distance(_first, _last));
		const size_type _size    = size();

		if (_pos1 > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		size_type _count2 = 0ul;

		for (; _first2 != _last2; ++_first2, ++_count2)
		{
			if (_count2 >= _count1)
			{
				traits_type::move(m_data.data() + _pos1 + _count2 + 1, m_data.data() + _pos1 + _count2,
								_size - _pos1 - _count1);
			}

			traits_type::assign(m_data[_pos1 + _count2], *_first2);
		}

		std::for_each(_first2, _last2, [&](CharT _chr) mutable {

			if (_count2 >= _count1)
			{
				traits_type::move(m_data.data() + _pos1 + _count2 + 1, m_data.data() + _pos1 + _count2,
								_size - _pos1 - _count1);
			}

			traits_type::assign(m_data[_pos1 + _count2], *_first2);

			_count2 = ~(-_count2);
		});

		const difference_type _new_bytes = static_cast<difference_type>(_count2 - _count1);
		const size_type _new_size        = static_cast<size_type>(_new_bytes) + _size;

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <class InputIt>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last, InputIt _first2,
														InputIt _last2, utils::is_input_iterator_tag)
		-> basic_inplace_string &
	{
		const size_type _pos1             = static_cast<size_type>(_first - m_data.data());
		const size_type _count1           = static_cast<size_type>(std::distance(_first, _last));

		const size_type _count2           = static_cast<size_type>(std::distance(_first2, _last2));
		const difference_type _new_bytes  = static_cast<difference_type>(_count2 - _count1);

		const size_type _size             = size();
		const size_type _new_size         = static_cast<size_type>(_new_bytes) + _size;

		if (_pos1 > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		if (_new_size > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::replace: exceed maximum string length");
		}

		traits_type::move(m_data.data() + _pos1 + _count2, m_data.data() + _pos1 + _count1, _size - _pos1 - _count1);

		pointer _ptr = m_data.data() + _pos1;

		std::for_each(_first2, _last2, [&](CharT _chr) mutable
		{
			traits_type::assign(*_ptr++, _chr);
		});
		
		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos1, size_type _count1, const CharT *_str,
														size_type _count2) -> basic_inplace_string &
	{
		const size_type _size             = size();
		const difference_type _new_bytes  = static_cast<difference_type>(_count2 - _count1);
		const size_type _new_size         = static_cast<size_type>(_new_bytes) + _size;

		if (_pos1 > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		if (_new_size > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::replace: exceed maximum string length");
		}

		traits_type::move(m_data.data() + _pos1 + _count2, m_data.data() + _pos1 + _count1, _size - _pos1);

		std::for_each_n(_str, _count2, [&, _i=0ul](CharT _chr) mutable
		{
			traits_type::assign(m_data[_pos1 + _i++], _chr);
		});
		
		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last, const CharT *_str,
														size_type _count2) -> basic_inplace_string &
	{
		const size_type _pos1    = static_cast<size_type>(_first - m_data.data());
		const size_type _count1  = static_cast<size_type>(std::distance(_first, _last));

		return replace(_pos1, _count1, _str, _count2);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos, size_type _count, const CharT *_str)
		-> basic_inplace_string &
	{
		return replace(_pos, _count, _str, traits_type::length(_str));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last, const CharT *_str)
		-> basic_inplace_string &
	{
		return replace(_first - m_data.data(), std::distance(_first, _last), _str, traits_type::length(_str));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos1, size_type _count1, size_type _count2,
														value_type _chr) -> basic_inplace_string &
	{
		const size_type _size             = size();
		const difference_type _new_bytes  = static_cast<difference_type>(_count2 - _count1);
		const size_type _new_size         = static_cast<size_type>(_new_bytes) + _size;

		if (_pos1 > _size)
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		if (_new_size > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::replace: exceed maximum string length");
		}

		traits_type::move((m_data.data() + _pos1 + _count2), (m_data.data() + _pos1 + _count1), (size() - _pos1 - _count1));
		traits_type::assign((m_data.data() + _pos1), _count2, _chr);

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);

		return *this;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last, size_type _count2,
														value_type _chr) -> basic_inplace_string &
	{
		const size_type _pos1    = static_cast<size_type>(_first - m_data.data());
		const size_type _count1  = static_cast<size_type>(std::distance(_first, _last));

		return replace(_pos1, _count1, _count2, _chr);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last,
														std::initializer_list<value_type> _ilits) -> basic_inplace_string &
	{
		const size_type _pos1    = static_cast<size_type>(_first - m_data.data());
		const size_type _count1  = static_cast<size_type>(std::distance(_first, _last));

		return replace(_pos1, _count1, _ilits.begin(), _ilits.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos, size_type _count,
														basic_string_view<CharT, Traits> _str_v) -> basic_inplace_string &
	{
		return replace(_pos, _count, _str_v.data(), _str_v.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::replace(const_iterator _first, const_iterator _last,
														basic_string_view<CharT, Traits> _str_v) -> basic_inplace_string &
	{
		const size_type _pos1    = static_cast<size_type>(_first - m_data.data());
		const size_type _count1  = static_cast<size_type>(std::distance(_first, _last));

		return replace(_pos1, _count1, _str_v.data(), _str_v.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	template <typename T>
		requires(std::convertible_to<const T &, basic_string_view<CharT, Traits>> &&
				!std::convertible_to<const T &, const CharT *>)
	auto basic_inplace_string<N, CharT, Traits>::replace(size_type _pos, size_type _count, const T &t, size_type _pos2,
														size_type _count2) -> basic_inplace_string &
	{
		basic_string_view<CharT, Traits> _view = t;

		if (_pos2 > _view.size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::replace: out of range");
		}

		return replace(_pos, _count, _view.data() + _pos2, std::min(_view.size() - _pos2, _count2));
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::substr(size_type _pos, size_type _count) const -> basic_inplace_string
	{
		if (_pos > size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::substr: out of range");
		}

		return {data() + _pos, std::min(_count, size() - _pos)};
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/** SEARCH METHD **/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::find(const basic_inplace_string &_other, size_type _pos) const noexcept ->
		typename basic_inplace_string::size_type
	{
		return find(_other.data(), _pos, _other.size());
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::find(const_pointer _str, size_type _pos, size_type _count) const noexcept
		-> typename basic_inplace_string::size_type
	{
		if ((_pos >= size()) || (_count == 0ul))
		{
			return npos;
		}

		const_pointer _res = utils::search_substring<CharT, Traits>(cbegin() + _pos, cend(), _str, _str + _count);

		return _res ? static_cast<size_type>(_res - cbegin()) : npos;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::find(const_pointer _str, size_type _pos) const noexcept ->
		typename basic_inplace_string::size_type
	{
		return find(_str, _pos, traits_type::length(_str));
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::find(value_type _chr, size_type _pos) const noexcept ->
		typename basic_inplace_string::size_type
	{
		const_pointer _res = traits_type::find(data() + _pos, size() - _pos, _chr);

		return _res ? static_cast<size_type>(_res - cbegin()) : npos;
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::find(basic_string_view<CharT, Traits> _str_v,
													size_type _pos) const noexcept ->
		typename basic_inplace_string::size_type
	{
		return find(_str_v.data(), _pos, _str_v.size());
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::copy(value_type *_dest, size_type _count, size_type _pos) const ->
		typename basic_inplace_string::size_type
	{
		if (_pos > size())
		{
			utils::throw_helper<std::out_of_range>("basic_inplace_string::copy: out of range");
		}

		size_type _index = _pos;

		std::for_each_n(std::begin(m_data) + _pos, _pos + std::min(size() - _pos, _count),
						[&](CharT _chr) mutable { traits_type::assign(*_dest, m_data[_index]); });

		return (_index - _pos);
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	void basic_inplace_string<N, CharT, Traits>::resize(size_type _size)
	{
		resize(_size, value_type{});
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	void basic_inplace_string<N, CharT, Traits>::resize(size_type _new_size, value_type _chr)
	{
		if (_new_size > max_size())
		{
			utils::throw_helper<std::length_error>("basic_inplace_string::resize: exceed maximum string length");
		}

		const size_type _size = size();

		if (static_cast<difference_type>(_new_size - _size) > 0)
		{
			traits_type::assign(&m_data[_size], _new_size - _size, _chr);
		}

		traits_type::assign(m_data[_new_size], value_type{});
		set_size(_new_size);
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	void basic_inplace_string<N, CharT, Traits>::swap(basic_inplace_string &_other) noexcept
	{
		basic_inplace_string _tmp(_other);
		_other = *this;
		*this  = _tmp;
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	/****/
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	void basic_inplace_string<N, CharT, Traits>::set_size(size_type _size) noexcept
	{
		assert(_size <= max_size());

		m_data[N] = static_cast<value_type>(N - _size);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::get_remaining_size() const noexcept ->
		typename basic_inplace_string::size_type
	{
		return static_cast<size_type>(m_data[N]);
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::front() -> reference
	{
		assert(!empty());

		return m_data[0ul];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::front() const -> const_reference
	{
		assert(!empty());

		return m_data[0ul];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::back() -> typename basic_inplace_string::reference
	{
		assert(!empty());

		return m_data[size() - 1ul];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::back() const -> typename basic_inplace_string::const_reference
	{
		assert(!empty());

		return m_data[size() - 1ul];
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::data() noexcept -> typename basic_inplace_string::pointer
	{
		return m_data.data();
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::data() const noexcept -> typename basic_inplace_string::const_pointer
	{
		return m_data.data();
	}
//	-----------------------------------------------------------------------------------------------
	template <std::size_t N, typename CharT, typename Traits>
	auto basic_inplace_string<N, CharT, Traits>::c_str() const noexcept -> typename basic_inplace_string::const_pointer
	{
		return m_data.data();
	}
//	-----------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------

	// END