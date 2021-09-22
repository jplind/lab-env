#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

struct shaderProgramSource
{
	string vertexSource;
	string fragmentSource;
};

struct shaderObject
{
	GLuint program;

	shaderObject(const string& filepath)
	{
		shaderProgramSource source = parseShaderFile(filepath);
		createProgram(source.vertexSource, source.fragmentSource);
	}

	shaderProgramSource parseShaderFile(const string& filePath)
	{
		ifstream stream(filePath);
		string line;
		stringstream ss[2];
		int type;

		while (getline(stream, line))
		{
			if (line.find("#shader") != string::npos)
			{
				if (line.find("vertex") != string::npos)
					type = 0;

				else if (line.find("fragment") != string::npos)
					type = 1;
			}

			else
				ss[type] << line << '\n';
		}

		return shaderProgramSource{ ss[0].str(), ss[1].str() };
	}

	GLuint compileShader(GLuint type, const string& source)
	{
		GLuint id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);
			cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << endl;
			cout << message << endl;
			glDeleteShader(id);
			delete[] message;
			return 0;
		}

		return id;
	}

	void createProgram(const string& vertexShader, const string& fragmentShader)
	{
		GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void use()
	{
		glUseProgram(program);
	}
};

