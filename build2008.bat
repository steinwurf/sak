@rem Invoke VS 2012 Command Prompt
@call "D:\Develop\VS2012\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86
@rem Set Python output to unbuffered
set VS_UNICODE_OUTPUT=
set PYTHONUNBUFFERED=true
python waf configure --bundle=ALL,-waf-tools --waf-tools-path="../external-waf-tools/" --bundle-path="../deps" --options=cxx_mkspec=cxx_msvc11_x86
python waf build
