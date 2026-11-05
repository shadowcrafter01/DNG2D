add_executable(
    DNG2D_test
    main.cpp
)

target_link_libraries(
    DNG2D_test
    PRIVATE
        DNG2D::DNG2D
)

target_compile_features(
    DNG2D_test
    PRIVATE
        cxx_std_20
)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)

if(WIN32)

    add_custom_command(
        TARGET DNG2D_test
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:DNG2D_test>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_image::SDL3_image>"
            "$<TARGET_FILE_DIR:DNG2D_test>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
            "$<TARGET_FILE_DIR:DNG2D_test>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
            "$<TARGET_FILE_DIR:DNG2D_test>"
    )

endif()