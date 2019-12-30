#pragma once

#include <string>

namespace Muse
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader* Create(const std::string& a_VertexSrc, const std::string& a_FragmentSrc);

        int test = 4;
    };

    static std::string s_VertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;
            
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    static std::string s_FragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";


    static std::string s_FlatColorVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            void main()
            {
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    static std::string s_FlatColorFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0f);
            }
        )";

    static std::string s_TextureVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;
            
            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    static std::string s_TextureFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform vec3 u_Color;

            void main()
            {
                color = vec4(v_TexCoord, 0.0f, 1.0f);
            }
        )";
}
