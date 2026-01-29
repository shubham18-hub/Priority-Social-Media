# Contributing to Priority Social Media Platform

We love your input! We want to make contributing to Priority Social Media Platform as easy and transparent as possible, whether it's:

- Reporting a bug
- Discussing the current state of the code
- Submitting a fix
- Proposing new features
- Becoming a maintainer

## Development Process

We use GitHub to host code, to track issues and feature requests, as well as accept pull requests.

## Pull Requests

Pull requests are the best way to propose changes to the codebase. We actively welcome your pull requests:

1. Fork the repo and create your branch from `main`.
2. If you've added code that should be tested, add tests.
3. If you've changed APIs, update the documentation.
4. Ensure the test suite passes.
5. Make sure your code lints.
6. Issue that pull request!

## Any contributions you make will be under the MIT Software License

In short, when you submit code changes, your submissions are understood to be under the same [MIT License](http://choosealicense.com/licenses/mit/) that covers the project. Feel free to contact the maintainers if that's a concern.

## Report bugs using GitHub's [issue tracker](https://github.com/shubham18-hub/Priority-Social-Media/issues)

We use GitHub issues to track public bugs. Report a bug by [opening a new issue](https://github.com/shubham18-hub/Priority-Social-Media/issues/new); it's that easy!

## Write bug reports with detail, background, and sample code

**Great Bug Reports** tend to have:

- A quick summary and/or background
- Steps to reproduce
  - Be specific!
  - Give sample code if you can
- What you expected would happen
- What actually happens
- Notes (possibly including why you think this might be happening, or stuff you tried that didn't work)

People *love* thorough bug reports. I'm not even kidding.

## Development Guidelines

### Code Style

#### For C Code (`fullcode_multimedia.c`):
- Use K&R style with 4-space indentation
- Follow single-file application architecture
- Use descriptive variable names
- Add comments for complex logic
- Ensure proper memory management (malloc/free)
- Handle errors gracefully

#### For Web Code (`frontend.html`):
- Use consistent indentation (4 spaces)
- Follow modern JavaScript ES6+ practices
- Use semantic HTML elements
- Maintain responsive CSS design
- Add comments for complex functions
- Ensure cross-browser compatibility

### Feature Development

#### Priority-Based Content System
- Maintain the core priority system for close friends
- Ensure priority indicators (⭐) are visible
- Test priority sorting in feeds and notifications

#### Multimedia Support
- Support the existing media types: image, video, audio
- Validate file types properly
- Handle file size limitations gracefully
- Maintain backward compatibility

#### Data Structures
- Preserve linked list implementations
- Maintain data persistence formats
- Ensure efficient algorithms (O(1), O(n), O(log n))
- Test with various data sizes

### Testing

#### Manual Testing Checklist
- [ ] User registration and login
- [ ] Post creation (text, image, video, audio)
- [ ] Follow/unfollow functionality
- [ ] Close friends management
- [ ] Priority feed display
- [ ] Notifications system
- [ ] Search functionality
- [ ] Data persistence
- [ ] Responsive design on mobile/desktop

#### C Version Testing
- [ ] Compile without warnings
- [ ] Memory leak testing with valgrind (if available)
- [ ] File I/O operations
- [ ] Menu navigation
- [ ] Data validation

### Documentation

- Update README.md for new features
- Add inline code comments
- Update API documentation if applicable
- Include examples for new functionality

## Feature Requests

We welcome feature requests! Please:

1. Check if the feature already exists
2. Search existing issues to avoid duplicates
3. Provide detailed description of the feature
4. Explain the use case and benefits
5. Consider implementation complexity

### Priority Features for Contribution

1. **Real-time Messaging System**
   - Direct messages between users
   - Message threading
   - Online status indicators

2. **Profile Pictures & Avatars**
   - Image upload for profiles
   - Avatar display in posts and lists
   - Image resizing and optimization

3. **Dark/Light Theme Toggle**
   - CSS custom properties for theming
   - User preference storage
   - Smooth theme transitions

4. **Export/Import Functionality**
   - JSON data export
   - Data backup and restore
   - Migration between versions

5. **Mobile App (React Native)**
   - Native mobile experience
   - Push notifications
   - Offline functionality

## Community Guidelines

### Be Respectful
- Use welcoming and inclusive language
- Be respectful of differing viewpoints and experiences
- Gracefully accept constructive criticism
- Focus on what is best for the community

### Be Collaborative
- Help others learn and grow
- Share knowledge and resources
- Provide constructive feedback
- Support fellow contributors

### Be Professional
- Keep discussions focused and on-topic
- Avoid personal attacks or harassment
- Respect project maintainers' decisions
- Follow the code of conduct

## Getting Help

- 📖 Read the [README.md](README.md) for project overview
- 🐛 Check [Issues](https://github.com/shubham18-hub/Priority-Social-Media/issues) for known problems
- 💬 Start a [Discussion](https://github.com/shubham18-hub/Priority-Social-Media/discussions) for questions
- 📧 Contact maintainers for sensitive issues

## Recognition

Contributors will be recognized in:
- README.md contributors section
- Release notes for significant contributions
- Special thanks in project documentation

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for contributing to Priority Social Media Platform! 🚀**

*Together, we're building a better way to prioritize meaningful social connections.*