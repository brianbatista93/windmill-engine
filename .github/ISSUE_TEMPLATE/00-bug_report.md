name: Bug report
about: Create a report to help us improve
title: '[Bug]: Title this bug report'
labels: 'bug'
body:
  - type: input
    id: version
    attributes:
      label: Version
      description: What version of the app are you using?
      placeholder: '1.0.0'
  - type: textarea
    id: what-happened
    attributes:
      label: What happened?
      description: A clear and concise description of what the bug is.
      placeholder: Bug description...
      validations:
      required: true
  - type: textarea
    id: steps-to-reproduce
    attributes:
      label: Steps to reproduce
      description: Steps to reproduce the behavior.
      placeholder: |
          1. Go to '...'
          2. Click on '....'
          3. Scroll down to '....'
          4. See error
      validations:
      required: true
  - type: textarea
    id: expected-behavior
    attributes:
      label: Expected behavior
      description: A clear and concise description of what you expected to happen.
      placeholder: Bug description...
      validations:
      required: true
  - type: checkboxes
    id: operating-systems
    attributes:
      label: Which operating systems have you used?
      description: You may select more than one.
      options:
        - label: macOS
        - label: Windows
        - label: Linux
  - type: checkboxes
    id: renderer-api
    attributes:
      label: Which rendering API have you used?
      description: You may select more than one.
      options:
        - label: DirectX 12
        - label: Vulkan
        - label: Metal
