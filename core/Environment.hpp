#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Extension.hpp"
#include "Scene.hpp"

/**
 * Define "env" as a shorthand to get the Envrionment instance.
 */
#ifndef env
#define env core::Environment::GetInstance()
#endif

namespace core
{
  class Environment
  {
    public:
      ~Environment();

      static Environment& GetInstance();

      bool Initialize(const char* aTitle, int aWidth, int aHeight);
      void Run();

      void LoadScene(Scene& aScene);

      template <typename EXTENSION>
      void RegisterExtension();

    protected:
      Environment();

    private:
      void Update();
      void Render();

      GLFWwindow* mWindow;

      static std::unique_ptr<Environment> mInstance;
      std::vector<std::unique_ptr<Extension>> mExtensions;
      Scene* mCurrentScene;

      bool mInitialized;
      int mInternalWidth;
      int mInternalHeight;
  };
}

#endif
