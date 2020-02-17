def Settings( **kwargs ):
    return {
        'flags' : [
            '-x',
            'c',
            '-Wall',
            '-Wextra',
            '-Werror',
            # libs
            '-lsystemd',
            '-pthread',
            # includes
            '-I./boards/template',
            '-I./common',
            '-I./socketCAN',
            '-I./CANopenNode',
            '-I./CANopenNode/stack',
            ],
        }
