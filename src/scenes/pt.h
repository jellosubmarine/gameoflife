#pragma once

#include "../common.h"

#include <random>
#include <vector>

struct Ray {
  Vec3 origin;
  Vec3 dir;

  int depth = 1;
};

inline std::random_device
    rd; // Will be used to obtain a seed for the random number engine
inline std::mt19937
    gen(rd()); // Standard mersenne_twister_engine seeded with rd()
inline std::uniform_real_distribution<float> dis(0.0f, 1.f);

struct Sampler {
  static float sample1D() { return dis(gen); }
  static Vec2 sample2D() { return {dis(gen), dis(gen)}; }
  static Vec3 sample3D() { return {dis(gen), dis(gen), dis(gen)}; }
};

struct MaterialSample {
  Radiance fr;
  Ray wi;
  float pdf;
};

// enum ObjectType {
//   SPHERE,
//   PLANE,
// };
struct Object;

struct Intersection {
  float distance = -1.f;
  Object const *object = nullptr;
  Vec3 x;
  Vec3 n;
  // Intersection() {}
  // Intersection(float dist, Object const *obj, Vec3 pos, Vec3 normal)
  //     : distance(dist), object(obj), x(pos), n(normal) {}
  operator bool() const { return valid(); }
  bool valid() const { return object != nullptr and distance > 0.f; }
  bool operator<(Intersection const &other) const {
    return valid() and (!other.valid() or distance < other.distance);
  }
};

struct Camera {
  Vec3 pos;
  Vec3 dir;
  float w, h;
  float fov;
  Ray castRay(Vec2 const &coord) const {
    Vec3 u = Vec3::UnitX() * fov;
    Vec3 v = Vec3::UnitY() * fov * h / w;
    Vec3 d = u * (coord.x() / w - .5) + v * (coord.y() / h - .5) + dir;
    return {pos, d.normalized()};
  }
};

struct Material {
  Radiance diffuse = Radiance::Zero();
  Radiance emittance = Radiance::Zero();

  Radiance Le(Intersection const &i, Ray const &wo) const { return emittance; }
  MaterialSample sample(Intersection const &i, Ray const &wo) const {
    MaterialSample ms;
    ms.fr = diffuse;
    Vec3 d = (Sampler::sample3D() * 2.f - Vec3::Ones()).normalized();
    if (d.dot(i.n) < 0)
      d = -d;
    ms.wi = {i.x + i.n * EPSILON, d, wo.depth + 1};
    ms.pdf = 1.f;
    return ms;
  }
};

struct Transformable {
  Vec3 pos = Vec3::Zero();
  Material mat;
  Transformable(){};
  Transformable(Vec3 pos_, Material mat_) : pos(pos_), mat(mat_){};
};

struct Sphere : public Transformable {
  float radius;
  Sphere(float rad, Vec3 pos_, Material mat_)
      : Transformable(pos_, mat_), radius(rad) {}
  Intersection intersect(Ray const &r) const {
    Vec3 L = pos - r.origin;
    float tca = L.dot(r.dir);
    if (tca < 0)
      return {};
    float d2 = L.dot(L) - tca * tca;
    if (d2 > radius * radius)
      return {};
    float thc = std::sqrt(radius * radius - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 > t1)
      std::swap(t0, t1);

    if (t0 < 0) {
      t0 = t1; // if t0 is negative, let's use t1 instead
      if (t0 < 0)
        return {}; // both t0 and t1 are negative
    }

    Vec3 x = r.origin + r.dir * t0;
    return {t0, nullptr, x, (x - pos).normalized()};
    ;
  }
};

struct Object {
  enum class ObjectType { SPHERE };
  ObjectType type;
  union {
    Sphere sphere;
  };
  template <typename T> Object(T &&obj);
  Object(Object const &obj) = default;
  Object(Object &obj) = default;
  ~Object(){};

  Intersection intersect(Ray const &r) const {
    Intersection ins;
    switch (type) {
    case ObjectType::SPHERE:
      ins = sphere.intersect(r);
      ins.object = this;
      return ins;
    default:
      abort();
    }
  }
  Material getMaterial() const {
    switch (type) {
    case ObjectType::SPHERE:
      return sphere.mat;
    default:
      abort();
    }
  }
};

template <>
inline Object::Object(Sphere &&obj)
    : type(ObjectType::SPHERE), sphere(std::forward<Sphere>(obj)) {}

struct Scene {
  std::vector<Object> objects;
  Intersection intersect(Ray const &r) const {
    Intersection ret;
    for (auto &o : objects) {
      auto test = o.intersect(r);
      if (test < ret)
        ret = test;
    }
    return ret;
  }
};

struct PathTracer {
  Color4 toSRGB(Radiance r, AppContext &ctx) const {
    r = r.array().min(1.f).max(0.f).pow(1.f / ctx.gamma) * 255.f;
    Color4 c;
    c << r.cast<uint8_t>(), 255;
    return c;
  }

  std::vector<Radiance> radianceBuffer;

  void reset(Camera const &cam) {
    radianceBuffer.resize(cam.w * cam.h, Radiance::Zero());
  }

  void render(Scene const &scene, Camera const &cam, AppContext &ctx,
              std::vector<Pixel> &image) {
    // 	foreach pixel in imageBuffer:
    size_t idx = 0;
    for (auto &pixel : image) {
      // 		for sample in range(0, N):
      // for (size_t sample = 0; sample < ctx.samples; ++sample)
      // 			pixel.radiance += trace(camera.castRay(pixel.xy))
      // / N
      radianceBuffer[idx] += trace(scene, cam.castRay(pixel.xy), ctx);

      // 		pixel.color = pixel.toSRGB()
      pixel.color =
          toSRGB((radianceBuffer[idx] / (ctx.frame + 1)) * ctx.exposure, ctx);
      idx++;
    }
  }

  bool shouldTerminate(Ray const &r, AppContext &ctx) const {
    return r.depth > ctx.max_depth;
  }

  Radiance trace(Scene const &scene, Ray const &wo, AppContext &ctx) const {
    // 	object = scene.intersect(x, wo)
    auto hit = scene.intersect(wo);
    // 	if not object	return Radiance(0)
    if (not hit)
      return Radiance::Zero();

    if (ctx.mode == MODE_NORMAL)
      return hit.n;
    if (ctx.mode == MODE_DEPTH) {
      Vec3 invDepthColor = Vec3::Ones() * hit.distance / ctx.far_plane;
      invDepthColor = invDepthColor.array().min(1.f).max(0.f);
      return Vec3::Ones() - invDepthColor;
    }
    // 	if shouldTerminate() return object.Le(x, wo)
    if (shouldTerminate(wo, ctx))
      return hit.object->getMaterial().Le(hit, wo);
    // 	wi = object.sampleRay(x, wo)
    auto ms = hit.object->getMaterial().sample(hit, wo);
    // 	Li = trace(x, wi)
    Radiance Li = trace(scene, ms.wi, ctx);
    // 	return object.Le(x, wo) + 2*PI * object.fr(x, wo, wi) * Li *
    // wi.dot(object.n)
    return hit.object->getMaterial().Le(hit, wo) +
           ms.fr.cwiseProduct(Li) * ms.wi.dir.dot(hit.n) / ms.pdf;
  }
};