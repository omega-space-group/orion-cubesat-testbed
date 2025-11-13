# ORION CubeSat Testbed - Versioning Guide

**Internal Document - Not for public repository**

This document tracks versioning strategy and milestones for the ORION CubeSat Testbed project.

---

## Current Version

**Version:** `0.1.0-alpha`  
**Status:** 🔄 Active Development  
**Last Updated:** [DATE]

---

## Versioning Strategy

We use **Semantic Versioning (SemVer)**: `MAJOR.MINOR.PATCH-STAGE`

### Format: `X.Y.Z-stage`

- **MAJOR (X)**: Breaking changes, major architectural shifts (0 = in development)
- **MINOR (Y)**: New features, subsystems completed
- **PATCH (Z)**: Bug fixes, small improvements, documentation updates
- **STAGE**: 
  - `alpha` - Early development, features incomplete
  - `beta` - Feature complete, testing phase
  - `rc` - Release candidate, ready for external validation
  - (none) - Stable release

### Version 0.x.x Rules

- Stay in `0.x.x` while in active development
- No API stability guarantees
- Breaking changes allowed between minor versions
- Move to `1.0.0` only when production-ready

---

## Planned Version Timeline

### Alpha Phase (0.1.0 - 0.6.0)

| Version | Target | Milestone | Status |
|---------|--------|-----------|--------|
| **0.1.0-alpha** | Current | Initial structure, documentation | ✅ Complete |
| **0.2.0-alpha** | Week 4 | C&DH basic functionality (commands, telemetry) | ⏳ Planned |
| **0.3.0-alpha** | Week 6 | CAN bus communication working | ⏳ Planned |
| **0.4.0-alpha** | Week 8 | Space ROS integration complete | ⏳ Planned |
| **0.5.0-alpha** | Week 10 | Payload containerization working | ⏳ Planned |
| **0.6.0-alpha** | Week 12 | End-to-end command flow (ground → satellite → payload) | ⏳ Planned |

### Beta Phase (0.7.0 - 0.9.0)

| Version | Target | Milestone | Status |
|---------|--------|-----------|--------|
| **0.7.0-beta** | Month 4 | All subsystems integrated, entering testing | ⏳ Planned |
| **0.8.0-beta** | Month 5 | Ground station operational | ⏳ Planned |
| **0.9.0-beta** | Month 6 | Full system tested, docs complete | ⏳ Planned |

### Release Candidate Phase (1.0.0-rc)

| Version | Target | Milestone | Status |
|---------|--------|-----------|--------|
| **1.0.0-rc1** | Month 7 | Release candidate, external testing | ⏳ Planned |
| **1.0.0-rc2** | TBD | Bug fixes from testing | ⏳ Planned |

### Stable Release (1.0.0+)

| Version | Target | Milestone | Status |
|---------|--------|-----------|--------|
| **1.0.0** | Month 8 | First stable release - fully functional testbed | ⏳ Planned |

---

## When to Update Version

### Bump MINOR (0.X.0)
- Complete a major subsystem
- Significant new functionality
- Major milestone reached
- Integration of new hardware component

**Examples:**
- C&DH can handle commands ✓
- CAN communication works ✓
- AI payload running first model ✓
- Ground station established ✓

### Bump PATCH (0.0.X)
- Bug fixes
- Small improvements
- Documentation updates
- Configuration changes
- Performance optimizations

**Examples:**
- Fix command parser bug
- Update README
- Optimize message handling
- Add unit tests

### Change STAGE
- **alpha → beta**: All planned subsystems integrated, begin testing
- **beta → rc**: Testing complete, system validated
- **rc → stable**: Release candidate approved, production-ready

---

## Git Tagging Procedure

When bumping version:

```bash
# 1. Update version in README.md
# Status: 🔄 Active Development | Version: 0.X.0-alpha

# 2. Update this file (VERSION_TRACKING.md)

# 3. Commit changes
git add README.md VERSION_TRACKING.md
git commit -m "chore: bump version to 0.X.0-alpha"

# 4. Create annotated tag
git tag -a v0.X.0-alpha -m "Version 0.X.0-alpha: [Brief description of milestone]"

# 5. Push commits and tags
git push origin main
git push origin v0.X.0-alpha
```

---

## Version History Log

### v0.1.0-alpha (Current)
**Date:** [DATE]  
**Changes:**
- Initial repository structure
- Complete documentation framework
- All subsystem directories created
- Contributing guidelines established
- GPL-3.0 license applied

**Milestone:** Project foundation complete

---

## Status Badges Reference

Update README badge as version progresses:

```markdown
# Alpha Phase
Status: 🔄 Active Development

# Beta Phase  
Status: 🧪 Testing Phase

# Release Candidate
Status: ✅ Release Candidate

# Stable
Status: ✅ Stable Release

# After v1.0.0, for mature project
Status: 📚 Maintenance Mode
```

---

## Next Version Planning

### Target: v0.2.0-alpha

**Goal:** Basic C&DH functionality operational

**Requirements:**
- [ ] C&DH can receive telecommands
- [ ] C&DH can send telemetry
- [ ] Basic mode management (Safe, Nominal, Payload)
- [ ] Housekeeping data collection working
- [ ] Unit tests for C&DH core functions

**Target Date:** [DATE]

**Blocking Issues:**
- (List any blockers)

**Dependencies:**
- Space ROS 2 installed
- Raspberry Pi 4 configured
- CAN interface setup (can wait for 0.3.0)

---

## Future Major Versions

### Version 1.x.x - Stable Testbed
- Fully functional flatsat
- All subsystems operational
- Comprehensive documentation
- External validation complete

### Version 2.x.x - Advanced Features (Future)
- STM32 migration complete
- CCSDS protocol implementation
- Advanced FDIR capabilities
- Multi-mission support

### Version 3.x.x - Production Platform (Future)
- Flight-ready code
- Certification-ready documentation
- Performance optimized
- Multi-satellite support

---

## Notes & Reminders

- Keep `VERSION_TRACKING.md` updated with each version bump
- Always create git tags for releases
- Document breaking changes clearly
- Update timelines based on actual progress
- Adjust targets as needed - this is a guide, not rigid

---

**Last Updated:** [DATE]  
**Updated By:** [NAME]
