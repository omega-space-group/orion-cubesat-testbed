# Contributing to ORION CubeSat Testbed

Thank you for your interest in contributing to the ORION CubeSat Testbed! This document provides guidelines and instructions for contributing to the project.

## 🌟 How Can I Contribute?

### Reporting Bugs
- Use the GitHub issue tracker
- Check if the bug has already been reported
- Provide detailed information: steps to reproduce, expected vs actual behavior, system info
- Include relevant logs and screenshots

### Suggesting Enhancements
- Open an issue with the tag `enhancement`
- Clearly describe the feature and its benefits
- Explain why this enhancement would be useful to the project

### Code Contributions
1. **Fork the repository**
2. **Create a feature branch** from `main`
3. **Make your changes** with clear, descriptive commits
4. **Test thoroughly** - ensure all tests pass
5. **Submit a pull request**

## 🔧 Development Setup

### Prerequisites
```bash
# Install dependencies
sudo apt update
sudo apt install -y build-essential cmake git python3-pip

# Install ROS 2 Humble (if not already installed)
# Follow: https://docs.ros.org/en/humble/Installation.html
```

### Setting Up Your Development Environment
```bash
# Clone your fork
git clone https://github.com/YOUR-USERNAME/orion-cubesat-testbed.git
cd orion-cubesat-testbed

# Add upstream remote
git remote add upstream https://github.com/ORIGINAL-OWNER/orion-cubesat-testbed.git

# Create a new branch
git checkout -b feature/your-feature-name
```

## 📝 Coding Standards

### General Guidelines
- Write clean, readable, and well-documented code
- Follow existing code style and conventions
- Add comments for complex logic
- Keep functions small and focused on a single task

### Python Code Style
- Follow [PEP 8](https://www.python.org/dev/peps/pep-0008/)
- Use meaningful variable and function names
- Add docstrings to all functions and classes
- Maximum line length: 100 characters

```python
def process_telemetry(data: dict) -> bool:
    """
    Process incoming telemetry data from satellite subsystems.
    
    Args:
        data: Dictionary containing telemetry data
        
    Returns:
        True if processing successful, False otherwise
    """
    # Implementation here
    pass
```

### C/C++ Code Style
- Follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Use meaningful names
- Add comments for complex algorithms
- Use `const` wherever possible

### Commit Messages
Follow the conventional commits specification:

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples:**
```
feat(payload): add jetson inference pipeline

Implements real-time inference using TensorRT on Jetson.
Includes basic image preprocessing and model loading.

Closes #123
```

```
fix(comms): resolve HackRF initialization race condition

The SDR was occasionally failing to initialize due to timing issue.
Added proper synchronization and retry logic.
```

## 🧪 Testing

### Before Submitting a Pull Request
- Run all existing tests: `./scripts/run_tests.sh`
- Add tests for new features
- Ensure code coverage doesn't decrease
- Test on actual hardware if possible

### Test Structure
```bash
# Unit tests
./simulation/unit-tests/

# Integration tests
./simulation/integration-tests/

# Hardware-in-the-loop tests
./simulation/scenarios/
```

## 📚 Documentation

### Code Documentation
- Add docstrings/comments to all public functions
- Update README files in relevant subdirectories
- Document any new dependencies

### Architecture Documentation
- Update architecture diagrams if adding new components
- Document design decisions in `docs/architecture/`
- Keep API documentation up to date in `docs/api/`

## 🔍 Pull Request Process

1. **Update Documentation**: Ensure all documentation reflects your changes
2. **Add Tests**: Include tests that cover your changes
3. **Update CHANGELOG**: Add an entry describing your changes (if applicable)
4. **Ensure CI Passes**: All automated checks must pass
5. **Request Review**: Tag relevant maintainers for review
6. **Address Feedback**: Respond to review comments promptly

### Pull Request Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Unit tests added/updated
- [ ] Integration tests added/updated
- [ ] Tested on hardware

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] No new warnings generated
- [ ] Tests pass locally
```

## 🎓 For Students

### First-Time Contributors
1. Start with issues labeled `good first issue`
2. Read the [Getting Started Guide](docs/getting-started/)
3. Join our communication channels (Slack/Discord/etc.)
4. Don't hesitate to ask questions!

### Learning Resources
- [Git Tutorial](https://git-scm.com/docs/gittutorial)
- [ROS 2 Documentation](https://docs.ros.org/)
- [Space ROS Documentation](https://space.ros.org/)

## 📋 Project Structure

Familiarize yourself with the repository structure:
- `flight-software/` - On-board software components
- `ground-segment/` - Ground station software
- `middleware/` - Space ROS integration
- `hardware/` - Hardware configurations
- `simulation/` - Testing infrastructure
- `docs/` - Documentation

## 🔐 Security

If you discover a security vulnerability:
1. **DO NOT** open a public issue
2. Email the maintainers directly at [svellas@mail.ntua.gr]
3. Provide detailed information about the vulnerability
4. Allow time for the issue to be addressed before public disclosure

## 📜 Code of Conduct

This project adheres to a Code of Conduct. By participating, you are expected to uphold this code. Please report unacceptable behavior to [svellas.@mail.ntua.gr].

## ❓ Questions?

- Check existing [Issues](https://github.com/YOUR-USERNAME/orion-cubesat-testbed/issues)
- Ask in our communication channels
- Contact the maintainers

## 🙏 Recognition

Contributors will be:
- Listed in the project README
- Acknowledged in research publications (where applicable)
- Credited in release notes

Thank you for contributing to open-source space technology! 🚀
