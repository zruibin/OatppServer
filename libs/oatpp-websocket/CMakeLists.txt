
message("oatpp websocket version was 1.3.0")

#[[
add_library(${OATPP_THIS_MODULE_NAME}
        oatpp-websocket/AsyncConnectionHandler.cpp
        oatpp-websocket/AsyncConnectionHandler.hpp
        oatpp-websocket/AsyncWebSocket.cpp
        oatpp-websocket/AsyncWebSocket.hpp
        oatpp-websocket/Config.hpp
        oatpp-websocket/ConnectionHandler.cpp
        oatpp-websocket/ConnectionHandler.hpp
        oatpp-websocket/Connector.cpp
        oatpp-websocket/Connector.hpp
        oatpp-websocket/Frame.cpp
        oatpp-websocket/Frame.hpp
        oatpp-websocket/Handshaker.cpp
        oatpp-websocket/Handshaker.hpp
        oatpp-websocket/SHA1.cpp
        oatpp-websocket/SHA1.hpp
        oatpp-websocket/Utils.cpp
        oatpp-websocket/Utils.hpp
        oatpp-websocket/WebSocket.cpp
        oatpp-websocket/WebSocket.hpp
)
]]

set(oatpp-websocket_name oatpp-websocket)

file(GLOB_RECURSE oatpp-websocket_all_file ${libs_dir}/oatpp-websocket/*)

if(USE_DEPENDS_MODEL)
        sourceGroup("/libs" ${oatpp-websocket_all_file})
else()
        add_library(${oatpp-websocket_name} ${oatpp-websocket_all_file})
        sourceGroup("/libs/oatpp-websocket" ${oatpp-websocket_all_file})
endif()