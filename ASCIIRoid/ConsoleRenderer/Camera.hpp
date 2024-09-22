#pragma once


#include <thread>
#include <vector>

#include "ASCIIRoid/Math/Vector2.hpp"
#include "ASCIIRoid/Math/Vector3.hpp"


namespace ConsoleRenderer
{
	class Camera
	{
	public:
		Camera(const Math::Vector2i& viewportSize, Math::Vector3f position)
			: m_position(position) , m_viewportSize(viewportSize)
		{
			RecalculateRayDirections();
		}

		float GetYaw() const { return m_yaw; }

		const Math::Vector3f& GetPosition() const { return m_position; }
		const Math::Vector3f& GetRayDirection(const Math::Vector2i& screenPos) const
		{
			return m_rayCache[screenPos.x + screenPos.y * m_viewportSize.x];
		}
		void OnResize(const Math::Vector2i& newSize)
		{
			m_viewportSize = newSize;
			RecalculateRayDirections();
		}

		void RotateY(float angle)
		{
			m_yaw += angle;
			if (m_yaw < 0)
			{
				m_yaw += 2 * 3.1415926535f;
			}
			if (m_yaw > 2 * 3.1415926535f)
			{
				m_yaw = 0;
			}
			RecalculateRayDirections();
		}

	private:

		void ThreadRayDir(int rx, int ry, int rwidth, int rheight)
		{
			for (int y = ry; y < ry + rheight; y++)
			{
				for (int x = rx; x < rx + rwidth; x++)
				{
					Math::Vector2f coord = {
						static_cast<float>(x) / static_cast<float>(m_viewportSize.x) * (static_cast<float>(m_viewportSize.x) / static_cast<float>(m_viewportSize.y)),
						static_cast<float>(y) / static_cast<float>(m_viewportSize.y)
					};
					coord = coord * 2.0f - 1.0f; // -1 -> 1

					float ar = m_viewportSize.x / static_cast<float>(m_viewportSize.y);

					float radFov = m_vFov * 3.1415928535f / 180.0f;

					float rayX = (2.0f * ((static_cast<float>(x) + 0.5f) /static_cast<float>(m_viewportSize.x)) - 1.0f) *  ar * tan(radFov / 2.0f);
					float rayY = (1.0f - 2.0f * ((static_cast<float>(y) + 0.5f) /static_cast<float>(m_viewportSize.y)) * tan(radFov / 2.0f));

					Math::Vector3f rayDir = {rayX, rayY, 1};

					float yaw = m_yaw + 3.1415926535f;

					rayDir.x = rayX * cos(yaw) + 1 * sin(yaw);
					rayDir.z = 1 * cos(yaw) - rayX * sin(yaw);
					
					m_rayCache[x + y * m_viewportSize.x] = rayDir.Normalized();
				}
			}
		}

		
		void RecalculateRayDirections()
		{
			m_rayCache.resize(m_viewportSize.x * m_viewportSize.y);
			std::vector<std::thread> threads;
			int nbThreads = 32;
			int partWidth = m_viewportSize.x / nbThreads;
			threads.reserve(nbThreads);
			for (int i = 0; i < nbThreads; i++)
			{
				threads.emplace_back(&Camera::ThreadRayDir, this, i * partWidth, 0, partWidth, m_viewportSize.y);
			}
			
			// wait for all threads to finish
			for (auto& thread : threads)
			{
				thread.join();
			}
		}


	private:
		float m_vFov = 90.f;

		Math::Vector3f m_position = Math::Vector3f(0.0f, 0.0f, 0.0f);
		float m_yaw = 0.0f;
		


		Math::Vector2i m_viewportSize = Math::Vector2i::zero;
		// Cached ray directions
		std::vector<Math::Vector3f> m_rayCache;
	};
}
