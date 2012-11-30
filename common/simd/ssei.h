// ======================================================================== //
// Copyright 2009-2012 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_SSEI_H__
#define __EMBREE_SSEI_H__

namespace embree
{
  /*! 4-wide SSE integer type. */
  struct ssei
  {
    typedef sseb Mask;                    // mask type for us
    enum   { size = 4 };                  // number of SIMD elements
    union  { __m128i m128; int32 v[4]; }; // data

    ////////////////////////////////////////////////////////////////////////////////
    /// Constructors, Assignment & Cast Operators
    ////////////////////////////////////////////////////////////////////////////////
    
    __forceinline ssei           ( ) {}
    __forceinline ssei           ( const ssei& a ) { m128 = a.m128; }
    __forceinline ssei& operator=( const ssei& a ) { m128 = a.m128; return *this; }

    __forceinline ssei( const __m128i a ) : m128(a) {}
    __forceinline operator const __m128i&( void ) const { return m128; }
    __forceinline operator       __m128i&( void )       { return m128; }

    __forceinline explicit ssei  ( const int32* const a ) : m128(_mm_loadu_si128((__m128i*)a)) {}
    //__forceinline ssei           ( int32  a ) : m128(_mm_set1_epi32(a)) {}
    __forceinline ssei           ( const int32& a ) : m128(_mm_shuffle_epi32(_mm_castps_si128(_mm_load_ss((float*)&a)), _MM_SHUFFLE(0, 0, 0, 0))) {}
    __forceinline ssei           ( int32  a, int32  b) : m128(_mm_set_epi32(b, a, b, a)) {}
    __forceinline ssei           ( int32  a, int32  b, int32  c, int32  d) : m128(_mm_set_epi32(d, c, b, a)) {}

    __forceinline explicit ssei( const __m128 a ) : m128(_mm_cvtps_epi32(a)) {}

    ////////////////////////////////////////////////////////////////////////////////
    /// Constants
    ////////////////////////////////////////////////////////////////////////////////

    __forceinline ssei( ZeroTy   ) : m128(_mm_setzero_si128()) {}
    __forceinline ssei( OneTy    ) : m128(_mm_set_epi32(1, 1, 1, 1)) {}
    __forceinline ssei( PosInfTy ) : m128(_mm_set_epi32(pos_inf, pos_inf, pos_inf, pos_inf)) {}
    __forceinline ssei( NegInfTy ) : m128(_mm_set_epi32(neg_inf, neg_inf, neg_inf, neg_inf)) {}
    __forceinline ssei( StepTy )   : m128(_mm_set_epi32(3, 2, 1, 0)) {}

    ////////////////////////////////////////////////////////////////////////////////
    /// Array Access
    ////////////////////////////////////////////////////////////////////////////////

    __forceinline const int32& operator []( const size_t i ) const { assert(i < 4); return v[i]; }
    __forceinline       int32& operator []( const size_t i )       { assert(i < 4); return v[i]; }
  };

  ////////////////////////////////////////////////////////////////////////////////
  /// Unary Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const ssei operator +( const ssei& a ) { return a; }
  __forceinline const ssei operator -( const ssei& a ) { return _mm_sub_epi32(_mm_setzero_si128(), a.m128); }
  __forceinline const ssei abs       ( const ssei& a ) { return _mm_abs_epi32(a.m128); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Binary Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const ssei operator +( const ssei& a, const ssei& b ) { return _mm_add_epi32(a.m128, b.m128); }
  __forceinline const ssei operator +( const ssei& a, const int32&  b ) { return a + ssei(b); }
  __forceinline const ssei operator +( const int32&  a, const ssei& b ) { return ssei(a) + b; }

  __forceinline const ssei operator -( const ssei& a, const ssei& b ) { return _mm_sub_epi32(a.m128, b.m128); }
  __forceinline const ssei operator -( const ssei& a, const int32&  b ) { return a - ssei(b); }
  __forceinline const ssei operator -( const int32&  a, const ssei& b ) { return ssei(a) - b; }

  __forceinline const ssei operator *( const ssei& a, const ssei& b ) { return _mm_mullo_epi32(a.m128, b.m128); }
  __forceinline const ssei operator *( const ssei& a, const int32&  b ) { return a * ssei(b); }
  __forceinline const ssei operator *( const int32&  a, const ssei& b ) { return ssei(a) * b; }

  __forceinline const ssei operator &( const ssei& a, const ssei& b ) { return _mm_and_si128(a.m128, b.m128); }
  __forceinline const ssei operator &( const ssei& a, const int32&  b ) { return a & ssei(b); }
  __forceinline const ssei operator &( const int32& a, const ssei& b ) { return ssei(a) & b; }

  __forceinline const ssei operator |( const ssei& a, const ssei& b ) { return _mm_or_si128(a.m128, b.m128); }
  __forceinline const ssei operator |( const ssei& a, const int32&  b ) { return a | ssei(b); }
  __forceinline const ssei operator |( const int32& a, const ssei& b ) { return ssei(a) | b; }

  __forceinline const ssei operator ^( const ssei& a, const ssei& b ) { return _mm_xor_si128(a.m128, b.m128); }
  __forceinline const ssei operator ^( const ssei& a, const int32&  b ) { return a ^ ssei(b); }
  __forceinline const ssei operator ^( const int32& a, const ssei& b ) { return ssei(a) ^ b; }

  __forceinline const ssei operator <<( const ssei& a, const int32& n ) { return _mm_slli_epi32(a.m128, n); }
  __forceinline const ssei operator >>( const ssei& a, const int32& n ) { return _mm_srai_epi32(a.m128, n); }

  __forceinline const ssei sra ( const ssei& a, const int32& b ) { return _mm_srai_epi32(a.m128, b); }
  __forceinline const ssei srl ( const ssei& a, const int32& b ) { return _mm_srli_epi32(a.m128, b); }
  
  __forceinline const ssei min( const ssei& a, const ssei& b ) { return _mm_min_epi32(a.m128, b.m128); }
  __forceinline const ssei min( const ssei& a, const int32&  b ) { return min(a,ssei(b)); }
  __forceinline const ssei min( const int32&  a, const ssei& b ) { return min(ssei(a),b); }

  __forceinline const ssei max( const ssei& a, const ssei& b ) { return _mm_max_epi32(a.m128, b.m128); }
  __forceinline const ssei max( const ssei& a, const int32&  b ) { return max(a,ssei(b)); }
  __forceinline const ssei max( const int32&  a, const ssei& b ) { return max(ssei(a),b); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Assignment Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline ssei& operator +=( ssei& a, const ssei& b ) { return a = a + b; }
  __forceinline ssei& operator +=( ssei& a, const int32&  b ) { return a = a + b; }
  
  __forceinline ssei& operator -=( ssei& a, const ssei& b ) { return a = a - b; }
  __forceinline ssei& operator -=( ssei& a, const int32&  b ) { return a = a - b; }
  
  __forceinline ssei& operator *=( ssei& a, const ssei& b ) { return a = a * b; }
  __forceinline ssei& operator *=( ssei& a, const int32&  b ) { return a = a * b; }
  
  __forceinline ssei& operator &=( ssei& a, const ssei& b ) { return a = a & b; }
  __forceinline ssei& operator &=( ssei& a, const int32&  b ) { return a = a & b; }
  
  __forceinline ssei& operator |=( ssei& a, const ssei& b ) { return a = a | b; }
  __forceinline ssei& operator |=( ssei& a, const int32&  b ) { return a = a | b; }
  
  __forceinline ssei& operator <<=( ssei& a, const int32&  b ) { return a = a << b; }
  __forceinline ssei& operator >>=( ssei& a, const int32&  b ) { return a = a >> b; }

  ////////////////////////////////////////////////////////////////////////////////
  /// Comparison Operators + Select
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const sseb operator ==( const ssei& a, const ssei& b ) { return _mm_castsi128_ps(_mm_cmpeq_epi32 (a.m128, b.m128)); }
  __forceinline const sseb operator ==( const ssei& a, const int32& b ) { return a == ssei(b); }
  __forceinline const sseb operator ==( const int32& a, const ssei& b ) { return ssei(a) == b; }
  
  __forceinline const sseb operator !=( const ssei& a, const ssei& b ) { return !(a == b); }
  __forceinline const sseb operator !=( const ssei& a, const int32& b ) { return a != ssei(b); }
  __forceinline const sseb operator !=( const int32& a, const ssei& b ) { return ssei(a) != b; }
  
  __forceinline const sseb operator < ( const ssei& a, const ssei& b ) { return _mm_castsi128_ps(_mm_cmplt_epi32 (a.m128, b.m128)); }
  __forceinline const sseb operator < ( const ssei& a, const int32& b ) { return a <  ssei(b); }
  __forceinline const sseb operator < ( const int32& a, const ssei& b ) { return ssei(a) <  b; }
  
  __forceinline const sseb operator >=( const ssei& a, const ssei& b ) { return !(a <  b); }
  __forceinline const sseb operator >=( const ssei& a, const int32& b ) { return a >= ssei(b); }
  __forceinline const sseb operator >=( const int32& a, const ssei& b ) { return ssei(a) >= b; }

  __forceinline const sseb operator > ( const ssei& a, const ssei& b ) { return _mm_castsi128_ps(_mm_cmpgt_epi32 (a.m128, b.m128)); }
  __forceinline const sseb operator > ( const ssei& a, const int32& b ) { return a >  ssei(b); }
  __forceinline const sseb operator > ( const int32& a, const ssei& b ) { return ssei(a) >  b; }

  __forceinline const sseb operator <=( const ssei& a, const ssei& b ) { return !(a >  b); }
  __forceinline const sseb operator <=( const ssei& a, const int32& b ) { return a <= ssei(b); }
  __forceinline const sseb operator <=( const int32& a, const ssei& b ) { return ssei(a) <= b; }
  
  /*! workaround for compiler bug in VS2008 */
#if defined(_MSC_VER) && (_MSC_VER < 1600)
  __forceinline const ssei select( const sseb& m, const ssei& a, const ssei& b ) { 
    return _mm_castps_si128(_mm_or_ps(_mm_and_ps(m, _mm_castsi128_ps(a)), _mm_andnot_ps(m, _mm_castsi128_ps(b)))); 
  }
#else
  __forceinline const ssei select( const sseb& m, const ssei& a, const ssei& b ) { 
    return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(b), _mm_castsi128_ps(a), m)); 
  }
#endif

  ////////////////////////////////////////////////////////////////////////////////
  // Movement/Shifting/Shuffling Functions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline ssei unpacklo( const ssei& a, const ssei& b ) { return _mm_castps_si128(_mm_unpacklo_ps(_mm_castsi128_ps(a.m128), _mm_castsi128_ps(b.m128))); }
  __forceinline ssei unpackhi( const ssei& a, const ssei& b ) { return _mm_castps_si128(_mm_unpackhi_ps(_mm_castsi128_ps(a.m128), _mm_castsi128_ps(b.m128))); }

  template<size_t i0, size_t i1, size_t i2, size_t i3> __forceinline const ssei shuffle( const ssei& a ) {
    return _mm_shuffle_epi32(a, _MM_SHUFFLE(i3, i2, i1, i0));
  }

  template<size_t i0, size_t i1, size_t i2, size_t i3> __forceinline const ssei shuffle( const ssei& a, const ssei& b ) {
    return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), _MM_SHUFFLE(i3, i2, i1, i0)));
  }

  template<> __forceinline const ssei shuffle<0, 0, 2, 2>( const ssei& a ) { return _mm_castps_si128(_mm_moveldup_ps(_mm_castsi128_ps(a))); }
  template<> __forceinline const ssei shuffle<1, 1, 3, 3>( const ssei& a ) { return _mm_castps_si128(_mm_movehdup_ps(_mm_castsi128_ps(a))); }
  template<> __forceinline const ssei shuffle<0, 1, 0, 1>( const ssei& a ) { return _mm_castpd_si128(_mm_movedup_pd (_mm_castsi128_pd(a))); }

  /*! workaround for compiler bug in VS2008 */
#if defined(_MSC_VER) && (_MSC_VER < 1600)
  template<size_t src> __forceinline int extract( const ssei& b ) { return b[src]; }
#else
  template<size_t src> __forceinline int extract( const ssei& b ) { return _mm_extract_epi32(b, src); }
#endif
  template<size_t dst> __forceinline const ssei insert( const ssei& a, const int32 b ) { return _mm_insert_epi32(a, b, dst); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Reductions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const ssei vreduce_min(const ssei& v) { ssei h = min(shuffle<1,0,3,2>(v),v); return min(shuffle<2,3,0,1>(h),h); }
  __forceinline const ssei vreduce_max(const ssei& v) { ssei h = max(shuffle<1,0,3,2>(v),v); return max(shuffle<2,3,0,1>(h),h); }
  __forceinline const ssei vreduce_add(const ssei& v) { ssei h = shuffle<1,0,3,2>(v)   + v ; return shuffle<2,3,0,1>(h)   + h ; }

  __forceinline int reduce_min(const ssei& v) { return extract<0>(vreduce_min(v)); }
  __forceinline int reduce_max(const ssei& v) { return extract<0>(vreduce_max(v)); }
  __forceinline int reduce_add(const ssei& v) { return extract<0>(vreduce_add(v)); }

  __forceinline size_t select_min(const ssei& v) { return __bsf(movemask(v == vreduce_min(v))); }
  __forceinline size_t select_max(const ssei& v) { return __bsf(movemask(v == vreduce_max(v))); }

  __forceinline size_t select_min(const sseb& valid, const ssei& v) { const ssei a = select(valid,v,ssei(pos_inf)); return __bsf(movemask(valid & (a == vreduce_min(a)))); }
  __forceinline size_t select_max(const sseb& valid, const ssei& v) { const ssei a = select(valid,v,ssei(neg_inf)); return __bsf(movemask(valid & (a == vreduce_max(a)))); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Output Operators
  ////////////////////////////////////////////////////////////////////////////////

  inline std::ostream& operator<<(std::ostream& cout, const ssei& a) {
    return cout << "<" << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ">";
  }
}

#endif

