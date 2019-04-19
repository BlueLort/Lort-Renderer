#pragma once
#include <immintrin.h>
#include <iostream>
#define ALIGN(x) _declspec(align(x))
class Vec4f
{
public:
	inline Vec4f() { vector = _mm_setzero_ps(); };
	inline Vec4f(float f) { vector = _mm_set1_ps(f); }
	inline Vec4f(float f1, float f2, float f3, float f4) { vector = _mm_set_ps(f4, f3, f2, f1); }
	inline Vec4f(const __m128& vec) { this->vector = vec; }
	inline __m128 getVec()const { return vector; }
	inline void setVec(const __m128& v) { vector = v; }
	inline void setVec(float f1,float f2,float f3,float f4) { vector = _mm_set_ps(f4, f3, f2, f1); }
	inline float getX()const { return vector.m128_f32[0]; }
	inline float getY()const { return vector.m128_f32[1]; }
	inline float getZ()const { return vector.m128_f32[2]; }
	inline float getW()const { return vector.m128_f32[3]; }
	inline void load(const float * p) { vector = _mm_loadu_ps(p); }
	inline void loada(const float * p) { vector = _mm_load_ps(p); }
	inline void store(float * p) { _mm_storeu_ps(p, vector); }
	inline void storea(float * p) { _mm_store_ps(p, vector); }
	inline float get(int32_t idx) { return vector.m128_f32[idx]; }
    inline float getMax() {
		__m128 max1 = _mm_permute_ps(vector, 0b00001110);
		__m128 max2 = _mm_max_ps(vector, max1);
		__m128 max3 = _mm_permute_ps(max2, 0b00000001);
		__m128 max4 = _mm_max_ps(max2, max3);
		return _mm_cvtss_f32(max4);
	}
	inline float getMin() {
		__m128 min1 = _mm_permute_ps(vector, 0b00001110);
		__m128 min2 = _mm_min_ps(vector, min1);
		__m128 min3 = _mm_permute_ps(min2, 0b00000001);
		__m128 min4 = _mm_min_ps(min2, min3);
		return _mm_cvtss_f32(min4);
	}
	inline float sum() {	
		return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(vector, _mm_setzero_ps()), _mm_setzero_ps()));
	}
	inline float length() {
		__m128 vecSqrd =_mm_mul_ps(vector,vector);
		return sqrt(_mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(vecSqrd, _mm_setzero_ps()), _mm_setzero_ps())));
	}
	inline Vec4f normalized() {
		float oneOverLength = 1.0f / this->length();
		return Vec4f(_mm_mul_ps(vector, _mm_set1_ps(oneOverLength)));
	}
	inline void insert(float value, uint32_t index) { vector.m128_f32[index] = value; }
	inline float operator[](int32_t index) const { return vector.m128_f32[index]; }
	inline ~Vec4f() {};
private:
	__m128 ALIGN(32) vector;

};


//Opertions
static inline Vec4f operator +(const Vec4f& v1, const Vec4f& v2) { return Vec4f(_mm_add_ps(v1.getVec(), v2.getVec())); }
static inline Vec4f operator +(const Vec4f& v, const float& f) { return Vec4f(_mm_add_ps(v.getVec(), _mm_set1_ps(f))); }
static inline Vec4f& operator +=(Vec4f& v1, const Vec4f& v2) { v1 = v1 + v2; return v1; }
static inline Vec4f operator -(const Vec4f& v1, const Vec4f& v2) { return Vec4f(_mm_sub_ps(v1.getVec(), v2.getVec())); }
static inline Vec4f operator -(const Vec4f& v, const float& f) { return Vec4f(_mm_sub_ps(v.getVec(), _mm_set1_ps(f))); }
static inline Vec4f& operator -=(Vec4f& v1, const Vec4f& v2) { v1 = v1 - v2; return v1; }
static inline Vec4f operator *(const Vec4f& v1, const Vec4f& v2) {return Vec4f(_mm_mul_ps(v1.getVec(), v2.getVec())); }
static inline Vec4f operator *(const Vec4f& v, const float& f) { return Vec4f(_mm_mul_ps(v.getVec(), _mm_set1_ps(f))); }
static inline Vec4f& operator *=(Vec4f& v1, const Vec4f& v2) { v1 = v1 * v2; return v1; }
static inline Vec4f operator /(const Vec4f& v1, const Vec4f& v2) { return Vec4f(_mm_div_ps(v1.getVec(), v2.getVec())); }
static inline Vec4f operator /(const Vec4f& v, const float& f) { return Vec4f(_mm_div_ps(v.getVec(), _mm_set1_ps(f))); }
static inline Vec4f& operator /=(Vec4f& v1, const Vec4f& v2) { v1 = v1 / v2; return v1; }

static inline Vec4f operator - (const Vec4f &v) { return _mm_sub_ps(_mm_setzero_ps(), v.getVec()); }
static inline Vec4f& operator ++(Vec4f &v) { v = v + 1; return v; }
static inline Vec4f  operator ++(Vec4f &v, int) { Vec4f ALIGN(32) v0(v); v = v + 1; return v0; }
static inline Vec4f& operator --(Vec4f &v) { v = v - 1; return v; }
static inline Vec4f  operator --(Vec4f &v, int) { Vec4f ALIGN(32) v0(v); v = v - 1; return v0; }

static inline Vec4f operator == (const Vec4f& v1, const Vec4f& v2) { return _mm_cmp_ps(v1.getVec(), v2.getVec(), 0); }
static inline Vec4f operator != (const Vec4f& v1, const Vec4f& v2) { return _mm_cmp_ps(v1.getVec(), v2.getVec(), 4); }
static inline Vec4f operator <(const Vec4f& v1, const Vec4f& v2) { return _mm_cmp_ps(v1.getVec(), v2.getVec(), 1); }
static inline Vec4f operator <= (const Vec4f& v1, const Vec4f& v2) { return _mm_cmp_ps(v1.getVec(), v2.getVec(), 2); }
static inline Vec4f operator >(const Vec4f& v1, const Vec4f& v2) { return _mm_cmp_ps(v2.getVec(), v1.getVec(), 1); }
static inline Vec4f operator >= (const Vec4f& v1, const Vec4f& v2) { return _mm_cmp_ps(v2.getVec(), v1.getVec(), 2); }

static inline Vec4f operator &(const Vec4f& v1, const Vec4f& v2) { return _mm_and_ps(v1.getVec(), v2.getVec()); }
static inline Vec4f operator && (const Vec4f& v1, const Vec4f& v2) { return _mm_and_ps(v1.getVec(), v2.getVec()); }

static inline Vec4f operator | (const Vec4f& v1, const Vec4f& v2) { return _mm_or_ps(v1.getVec(), v2.getVec()); }
static inline Vec4f operator || (const Vec4f& v1, const Vec4f& v2) { return _mm_or_ps(v1.getVec(), v2.getVec()); }

static inline Vec4f operator ^ (const Vec4f& v1, const Vec4f& v2) { return _mm_xor_ps(v1.getVec(), v2.getVec()); }

static inline Vec4f operator !(const Vec4f& v) { return _mm_cmp_ps(v.getVec(), _mm_setzero_ps(), 0); }


static inline Vec4f andnot(const Vec4f& v1, const Vec4f& v2) { return _mm_andnot_ps(v2.getVec(), v1.getVec()); }
static inline std::ostream& operator << (std::ostream& output, const Vec4f& v) { output << "Vec4f: ["; for (int32_t i = 0; i<4; i++) output << v.getVec().m128_f32[i] << ","; output << "]"; return output; }

static inline Vec4f if_select(const Vec4f& v1, const Vec4f& v2, const Vec4f& s) { return _mm_blendv_ps(v2.getVec(), v1.getVec(), s.getVec()); }
static inline Vec4f if_add(const Vec4f& v1, const Vec4f& v2, Vec4f const &f) { return v1 + (f&v2); }
static inline Vec4f if_sub(const Vec4f& v1, const Vec4f& v2, Vec4f const &f) { return v1 - (f&v2); }

template <int i0>
static inline __m128 const4f() {
	static const union { int i[4]; __m128 ymm; } u = { { i0,i0,i0,i0 } };
	return u.ymm;
}
static inline Vec4f abs(Vec4f const &v) {
	__m128 ALIGN(32) mask = const4f<0x7FFFFFFF>();
	return Vec4f(_mm_and_ps(v.getVec(), mask));
}

static inline Vec4f max(const Vec4f& v1, const Vec4f& v2) { return _mm_max_ps(v1.getVec(), v2.getVec()); }
static inline Vec4f min(const Vec4f& v1, const Vec4f& v2) { return _mm_min_ps(v1.getVec(), v2.getVec()); }

static inline Vec4f sqrt(const Vec4f& v) { return _mm_sqrt_ps(v.getVec()); }

static inline Vec4f round(const Vec4f& v) { return _mm_round_ps(v.getVec(), 0 + 8); }

static inline Vec4f truncate(const Vec4f& v) { return _mm_round_ps(v.getVec(), 3 + 8); }

static inline Vec4f floor(const Vec4f& v) { return _mm_round_ps(v.getVec(), 1 + 8); }

static inline Vec4f ceil(const Vec4f& v) { return _mm_round_ps(v.getVec(), 2 + 8); }


static inline float dot(const Vec4f& v1, const Vec4f& v2) {
	Vec4f dest(v1*v2);
	return dest.sum();
}

static inline Vec4f cross(const Vec4f& v1, const Vec4f& v2) {
	// 0b11001001 -> 3 0 2 1
	__m128 v1_yzx = _mm_permute_ps(v1.getVec(), 0b11001001);
	__m128 v2_yzx = _mm_permute_ps(v2.getVec(), 0b11001001);
	__m128 c = _mm_sub_ps(_mm_mul_ps(v1.getVec(), v2_yzx), _mm_mul_ps(v1_yzx, v2.getVec()));
	return Vec4f(_mm_permute_ps(c, 0b11001001));
}

static inline Vec4f lerp(const Vec4f& v1, const Vec4f& v2,float f) {
	return ((v2 - v1)*f + v1);
}