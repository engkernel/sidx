# sidx
### Experimenting with low level concepts of kernel
- x86
- clang-19
- Makefile
- c17
- ATT Assembly

### Files
- config.mk: variables needed for compile, build directory
- mk/common.mk: validations of exist of variables to prevent destructive actions(like removing $(BUILD_DIR)/boot in case BUILD_DIR has not been set)

# Notes
- With multiple try, I saw fault in my folder structure as it was more monolothic and heavy or nested. So I review structure and focus on minimalistic design.
