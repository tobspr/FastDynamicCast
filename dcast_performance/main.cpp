
#include "../fast_dynamic_cast.h"

#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>

// Helper method to measure output, returns duration in milliseconds
long long measure(const std::string& topic, const std::function<void()>& body) {
  std::cout << "Measure " << topic << ": ";
  auto begin = std::chrono::high_resolution_clock::now();
  body();
  auto end = std::chrono::high_resolution_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
  std::cout << ms << " ms" << std::endl;
  return ms;
}

int main() {

  size_t num_iterations = 2000000;

  // Performance test 1, simple class hierarchy
  // 
  //  A
  //  |
  //  B
  {

    struct A {
      virtual ~A() {};
    };

    struct B : public A {
      virtual int method_b_only() { return 3; }
    };

    measure("Regular dynamic cast simple", [&]() {
      volatile size_t accumulated = 0;
      for (size_t i = 0; i < num_iterations; ++i) {
        B b;
        A& a = b;
        accumulated += dynamic_cast<B&>(a).method_b_only();
      }
    });

    measure("Fast dynamic cast simple", [&]() {
      volatile size_t accumulated = 0;
      for (size_t i = 0; i < num_iterations; ++i) {
        B b;
        A& a = b;
        accumulated += fast_dynamic_cast<B&>(a).method_b_only();
      }
    });
  }

  // Performance test 2, complex class hierarchy with diamond shape
  //   
  //      A
  //      |
  //      B
  //      | \  
  //      C  E
  //      |  |
  //      D  F
  //       \/
  //        G
  {


    struct A {
      virtual ~A() {};
      virtual int method() { return 1; }
    };
    struct B : public A {
      virtual int method() { return 2; }
    };
    struct C : public virtual B {
      virtual int method() { return 3; }
    };
    struct D : public C {
      virtual int method() { return 4; }
    };
    struct E : public virtual B {
      virtual int method() { return 5; }
    };
    struct F : public E {
      virtual int method() { return 6; }
    };
    struct G : public D, public F {
      virtual int method() { return 7; }
      virtual int method_g_only() { return method(); }
    };

    measure("Regular dynamic cast complex", [&]() {
      volatile size_t accumulated = 0;
      for (size_t i = 0; i < num_iterations; ++i) {
        G g;
        A& a = g;
        accumulated += dynamic_cast<G&>(a).method_g_only();
      }
    });

    measure("Fast dynamic cast complex", [&]() {
      volatile size_t accumulated = 0;
      for (size_t i = 0; i < num_iterations; ++i) {
        G g;
        A& a = g;
        accumulated += fast_dynamic_cast<G&>(a).method_g_only();
      }
    });


    measure("Threaded regular dynamic cast complex", [&]() {

      auto runner = [&]() {
        volatile size_t accumulated = 0;
        for (size_t i = 0; i < num_iterations; ++i) {
          G g;
          A& a = g;
          accumulated += dynamic_cast<G&>(a).method_g_only();
        }
      };
      std::thread thread_a(runner);
      std::thread thread_b(runner);
      thread_a.join();
      thread_b.join();
    });


    measure("Threaded fast dynamic cast complex", [&]() {

      auto runner = [&]() {
        volatile size_t accumulated = 0;
        for (size_t i = 0; i < num_iterations; ++i) {
          G g;
          A& a = g;
          accumulated += fast_dynamic_cast<G&>(a).method_g_only();
        }
      };
      std::thread thread_a(runner);
      std::thread thread_b(runner);
      thread_a.join();
      thread_b.join();
    });

  }

}