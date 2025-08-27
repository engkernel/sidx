# sidx (serve the machine need)
## Fix Required
- I need to handle high half kernel from start, so paging and setup other objects are easier to manage.
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
- folder and structure design now are more shaped, simple and easier to understand. I prefer a big file but documented well and structured, than multiple seperated file that I should do jumpy jump between them to fix or read. 
