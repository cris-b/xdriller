
configure_file(config.cfg.in  config.cfg  @ONLY)
configure_file(plugins.cfg.in plugins.cfg @ONLY)
configure_file(resources.cfg.in resources.cfg @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.cfg 
              ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg
              ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/xdriller/default_config)
