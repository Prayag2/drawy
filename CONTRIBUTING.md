# Contributing to Drawy
Thank you for taking the time to contribute. All contributions are valued, and we welcome ideas, fixes, improvements, and discussions. This guide explains how to participate effectively and respectfully.

If you enjoy the project but cannot contribute code, you can still support it:

* Star the repository
* Share it with others
* Mention it in related projects or discussions

## Table of Contents
* [I Want To Contribute](#i-want-to-contribute)
* [Reporting Bugs](#reporting-bugs)
* [Suggesting Enhancements](#suggesting-enhancements)
* [Your First Code Contribution](#your-first-code-contribution)
* [Improving the Documentation](#improving-the-documentation)
* [Style Guidelines](#style-guidelines)
* [Commit Messages](#commit-messages)
* [Join the Project Team](#join-the-project-team)

## I Want To Contribute
When contributing, ensure that all submitted work is your own and can be distributed under the project license.

## Reporting Bugs
Before filing a bug report:

* Confirm the issue occurs on the latest version
* Search existing issues
* Collect details such as OS, compiler version, steps to reproduce, and any relevant output

To report a bug:

* Open a new issue
* Describe what you expected to happen and what actually happened
* Provide reproducible steps and all collected information

The team will review the report, attempt reproduction, and label it appropriately.

## Suggesting Enhancements
Before proposing an enhancement:

* Confirm it does not already exist
* Ensure it aligns with the project scope

To suggest an enhancement:

* Open an issue with a clear title
* Explain the motivation, expected behavior, and current limitations
* Include examples or screenshots if relevant

## Your First Code Contribution
If you are new to contributing:

* Start with issues labeled "good first issue" or "help wanted"
* Ask questions in the issue if anything is unclear

## Improving the Documentation
Documentation improvements are welcome. Update or extend documentation when you modify behavior or add new features.

## Pull Requests
* Open an issue before starting a pull request.
* Discuss the proposed solution in detail.
* Only begin work once the approach is approved.
* Ensure the project builds without warnings.
* Add tests where appropriate.

## Style Guidelines
### C++ Code
* Format all C++ code with `clang-format` before submitting. Using `nix develop` installs a git hook that formats code automatically on pre commit.
* Always use brace initialization such as `int a{5}`.
* Use smart pointers instead of raw pointers.
* Write clear and descriptive variable names.
* Add comments that help explain the intent behind code.
* Do not use [magic numbers](https://stackoverflow.com/a/47902).

### Licensing
* New source files must include the [LICENSE_HEADER](./LICENSE_HEADER) comment at the top.
* Append your name and email to the comment's `Authors` section.

## Commit Messages
Use the Conventional Commits standard.  
See [https://www.conventionalcommits.org](https://www.conventionalcommits.org)

## Join the Project Team
Active contributors may be invited to join the project team. Consistency, quality, and constructive communication are key.

## Attribution
This guide takes inspiration from [contributing.md](https://contributing.md).
