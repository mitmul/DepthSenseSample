#ifndef CPP_GLUT_INCLUDE
#define CPP_GLUT_INCLUDE

#include <gl\glut.h>
#include <gl\freeglut_ext.h>
#include <boost\shared_ptr.hpp>

namespace glut {

	//------------------------------------------------------//
	// template <class F>                                   //
	// void DisplayFunc(F f);                               //
	//                                                      //
	// glutDisplayFuncのテンプレート版                      //
	//   fはvoid f()のシグニチャを持つ関数(オブジェクト)    //
	//------------------------------------------------------//
	namespace display_detail {

		class functor_base {
		public:
			virtual void invoke() = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke()
			{
				f_();
			}
		};

		static boost::shared_ptr<functor_base> display_functor_;

		inline void dummyDisplayFunc()
		{
			display_functor_->invoke();
		}

	} // namespace display_detail

	template <class F>
	inline void DisplayFunc(F f)
	{
		using namespace display_detail;

		display_functor_.reset(new functor<F>(f));
		::glutDisplayFunc(dummyDisplayFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void OverlayDisplayFunc(F f);                        //
	//                                                      //
	// glutOverlayDisplayFuncのテンプレート版               //
	//   fはvoid f()のシグニチャを持つ関数(オブジェクト)    //
	//------------------------------------------------------//
	namespace display_detail {

		static boost::shared_ptr<functor_base> overlay_display_functor_;

		inline void dummyOverlayDisplayFunc()
		{
			overlay_display_functor_->invoke();
		}

	} // namespace display_detail

	template <class F>
	inline void OverlayDisplayFunc(F f)
	{
		using namespace display_detail;

		overlay_display_functor_.reset(new functor<F>(f));
		::glutDisplayFunc(dummyOverlayDisplayFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void ReshapeFunc(F f);                               //
	//                                                      //
	// glutReshapeFuncのテンプレート版                      //
	//   fはvoid f(int width, int height)の                 //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace reshape_detail {

		class functor_base {
		public:
			virtual void invoke(int width, int height) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int width, int height)
			{
				f_(width, height);
			}
		};

		static boost::shared_ptr<functor_base> reshape_functor_;

		inline void dummyReshapeFunc(int width, int height)
		{
			reshape_functor_->invoke(width, height);
		}

	} // namespace reshape_detail

	template <class F>
	inline void ReshapeFunc(F f)
	{
		using namespace reshape_detail;

		reshape_functor_.reset(new functor<F>(f));
		::glutReshapeFunc(dummyReshapeFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void MotionFunc(F f);                                //
	//                                                      //
	// glutMotionFuncのテンプレート版                       //
	//   fはvoid f(int x, int y)の                          //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace motion_detail {

		class functor_base {
		public:
			virtual void invoke(int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int x, int y)
			{
				f_(x, y);
			}
		};

		static boost::shared_ptr<functor_base> motion_functor_;

		inline void dummyMotionFunc(int x, int y)
		{
			motion_functor_->invoke(x, y);
		}

	} // namespace motion_detail

	template <class F>
	inline void MotionFunc(F f)
	{
		using namespace motion_detail;

		motion_functor_.reset(new functor<F>(f));
		::glutMotionFunc(dummyMotionFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void PassiveMotionFunc(F f);                         //
	//                                                      //
	// glutPassiveMotionFuncのテンプレート版                //
	//   fはvoid f(int x, int y)の                          //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace motion_detail {

		static boost::shared_ptr<functor_base> passive_motion_functor_;

		inline void dummyPassiveMotionFunc(int x, int y)
		{
			passive_motion_functor_->invoke(x, y);
		}

	} // namespace motion_detail

	template <class F>
	inline void PassiveMotionFunc(F f)
	{
		using namespace motion_detail;

		passive_motion_functor_.reset(new functor<F>(f));
		::glutPassiveMotionFunc(dummyPassiveMotionFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void KeyboardFunc(F f);                              //
	//                                                      //
	// glutKeyboardFuncのテンプレート版                     //
	//   fはvoid f(unsigned char key, int x, int y)の       //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace keyboard_detail {

		class functor_base {
		public:
			virtual void invoke(unsigned char key, int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(unsigned char key, int x, int y)
			{
				f_(key, x, y);
			}
		};

		static boost::shared_ptr<functor_base> keyboard_functor_;

		inline void dummyKeyboardFunc(unsigned char key, int x, int y)
		{
			keyboard_functor_->invoke(key, x, y);
		}

	} // namespace keyboard_detail

	template <class F>
	inline void KeyboardFunc(F f)
	{
		using namespace keyboard_detail;

		keyboard_functor_.reset(new functor<F>(f));
		::glutKeyboardFunc(dummyKeyboardFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void MouseFunc(F f);                                 //
	//                                                      //
	// glutMouseFuncのテンプレート版                        //
	//   fはvoid f(int button, int state, int x, int y)の   //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace mouse_detail {

		class functor_base {
		public:
			virtual void invoke(int button, int state, int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int button, int state, int x, int y)
			{
				f_(button, state, x, y);
			}
		};

		static boost::shared_ptr<functor_base> mouse_functor_;

		inline void dummyMouseFunc(int button, int state, int x, int y)
		{
			mouse_functor_->invoke(button, state, x, y);
		}

	} // namespace mouse_detail

	template <class F>
	inline void MouseFunc(F f)
	{
		using namespace mouse_detail;

		mouse_functor_.reset(new functor<F>(f));
		::glutMouseFunc(dummyMouseFunc);
	}

		
	//------------------------------------------------------//
	// template <class F>                                   //
	// void MouseWheelFunc(F f);                            //
	//                                                      //
	// glutMouseWheelFuncのテンプレート版                   //
	//   fはvoid f(int button, int dir, int x, int y)の     //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace mouse_wheel_detail {

		class functor_base {
		public:
			virtual void invoke(int button, int dir, int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int button, int dir, int x, int y)
			{
				f_(button, dir, x, y);
			}
		};

		static boost::shared_ptr<functor_base> mouse_wheel_functor_;

		inline void dummyMouseWheelFunc(int button, int dir, int x, int y)
		{
			mouse_wheel_functor_->invoke(button, dir, x, y);
		}
	} // namespace mouse_wheel_detail

	template <class F>
	inline void MouseWheelFunc(F f)
	{
		using namespace mouse_wheel_detail;

		mouse_wheel_functor_.reset(new functor<F>(f));
		::glutMouseWheelFunc(dummyMouseWheelFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void VisibilityFunc(F f);                            //
	//                                                      //
	// glutVisibilityFuncのテンプレート版                   //
	//   fはvoid f(int state)の                             //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace state_detail {

		class functor_base {
		public:
			virtual void invoke(int state) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int state)
			{
				f_(state);
			}
		};

		static boost::shared_ptr<functor_base> visibility_functor_;

		inline void dummyVisibilityFunc(int state)
		{
			visibility_functor_->invoke(state);
		}

	} // namespace state_detail

	template <class F>
	inline void VisibilityFunc(F f)
	{
		using namespace state_detail;

		visibility_functor_.reset(new functor<F>(f));
		::glutVisibilityFunc(dummyVisibilityFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void EntryFunc(F f);                                 //
	//                                                      //
	// glutEntryFuncのテンプレート版                        //
	//   fはvoid f(int state)の                             //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace state_detail {

		static boost::shared_ptr<functor_base> entry_functor_;

		inline void dummyEntryFunc(int state)
		{
			entry_functor_->invoke(state);
		}

	} // namespace state_detail

	template <class F>
	inline void EntryFunc(F f)
	{
		using namespace state_detail;

		entry_functor_.reset(new functor<F>(f));
		::glutEntryFunc(dummyEntryFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void SpecialFunc(F f);                               //
	//                                                      //
	// glutSpecialFuncのテンプレート版                      //
	//   fはvoid f(int key, int x, int y)の                 //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace special_detail {

		class functor_base {
		public:
			virtual void invoke(int key, int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int key, int x, int y)
			{
				f_(key, x, y);
			}
		};

		static boost::shared_ptr<functor_base> special_functor_;

		inline void dummySpecialFunc(int key, int x, int y)
		{
			special_functor_->invoke(key, x, y);
		}

	} // namespace special_detail

	template <class F>
	inline void SpecialFunc(F f)
	{
		using namespace special_detail;

		special_functor_.reset(new functor<F>(f));
		::glutSpecialFunc(dummySpecialFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void SpaceballMotionFunc(F f);                       //
	//                                                      //
	// glutSpaceballMotionFuncのテンプレート版              //
	//   fはvoid f(int x, int y, int z)の                   //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace vector3_detail {

		class functor_base {
		public:
			virtual void invoke(int x, int y, int z) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int x, int y, int z)
			{
				f_(x, y, z);
			}
		};

		static boost::shared_ptr<functor_base> spaceball_motion_functor_;

		inline void dummySpaceballMotionFunc(int x, int y, int z)
		{
			spaceball_motion_functor_->invoke(x, y, z);
		}

	} // namespace vector3_detail

	template <class F>
	inline void SpaceballMotionFunc(F f)
	{
		using namespace vector3_detail;

		spaceball_motion_functor_.reset(new functor<F>(f));
		::glutSpaceballMotionFunc(dummySpaceballMotionFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void SpaceballRotateFunc(F f);                       //
	//                                                      //
	// glutSpaceballRotateFuncのテンプレート版              //
	//   fはvoid f(int x, int y, int z)の                   //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace vector3_detail {

		static boost::shared_ptr<functor_base> spaceball_rotate_functor_;

		inline void dummySpaceballRotateFunc(int x, int y, int z)
		{
			spaceball_rotate_functor_->invoke(x, y, z);
		}

	} // namesapce vector3_detail

	template <class F>
	inline void SpaceballRotateFunc(F f)
	{
		using namespace vector3_detail;

		spaceball_rotate_functor_.reset(new functor<F>(f));
		::glutSpaceballRotateFunc(dummySpaceballRotateFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void SpaceballButtonFunc(F f);                       //
	//                                                      //
	// glutSpaceballButtonFuncのテンプレート版              //
	//   fはvoid f(int button, int state)の                 //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace button_state_detail {

		class functor_base {
		public:
			virtual void invoke(int button, int state) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int button, int state)
			{
				f_(button, state);
			}
		};

		static boost::shared_ptr<functor_base> spaceball_button_functor_;

		inline void dummySpaceballButtonFunc(int button, int state)
		{
			spaceball_button_functor_->invoke(button, state);
		}

	} // namespace button_state_detail

	template <class F>
	inline void SpaceballButtonFunc(F f)
	{
		using namespace button_state_detail;

		spaceball_button_functor_.reset(new functor<F>(f));
		::glutSpaceballButtonFunc(dummySpaceballButtonFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void ButtonBoxFunc(F f);                             //
	//                                                      //
	// glutButtonBoxFuncのテンプレート版                    //
	//   fはvoid f(int button, int state)の                 //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace button_state_detail {

		static boost::shared_ptr<functor_base> button_box_functor_;

		inline void dummyButtonBoxFunc(int button, int state)
		{
			button_box_functor_->invoke(button, state);
		}

	} // namespace button_state_detail

	template <class F>
	inline void ButtonBoxFunc(F f)
	{
		using namespace button_state_detail;

		button_box_functor_.reset(new functor<F>(f));
		::glutButtonBoxFunc(dummyButtonBoxFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void DialsFunc(F f);                                 //
	//                                                      //
	// glutDialsFuncのテンプレート版                        //
	//   fはvoid f(int dials, int value)の                  //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace dials_detail {

		class functor_base {
		public:
			virtual void invoke(int dials, int value) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int dials, int value)
			{
				f_(dials, value);
			}
		};

		static boost::shared_ptr<functor_base> dials_functor_;

		inline void dummyDialsFunc(int dials, int value)
		{
			dials_functor_->invoke(dials, value);
		}

	} // namespace dials_detail

	template <class F>
	inline void DialsFunc(F f)
	{
		using namespace dials_detail;

		dials_functor_.reset(new functor<F>(f));
		::glutDialsFunc(dummyDialsFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void TabletMotionFunc(F f);                          //
	//                                                      //
	// glutTabletMotionFuncのテンプレート版                 //
	//   fはvoid f(int x, int y)の                          //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace tablet_motion_detail {

		class functor_base {
		public:
			virtual void invoke(int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int x, int y)
			{
				f_(x, y);
			}
		};

		static boost::shared_ptr<functor_base> tablet_motion_functor_;

		inline void dummyTabletMotionFunc(int x, int y)
		{
			tablet_motion_functor_->invoke(x, y);
		}

	} // namespace tablet_motion_detail

	template <class F>
	inline void TabletMotionFunc(F f)
	{
		using namespace tablet_motion_detail;

		tablet_motion_functor_.reset(new functor<F>(f));
		::glutTabletMotionFunc(dummyTabletMotionFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void TabletButtonFunc(F f);                          //
	//                                                      //
	// glutTabletButtonFuncのテンプレート版                 //
	//   fはvoid f(int button, int state, int x, int y)の   //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace tablet_button_detail {

		class functor_base {
		public:
			virtual void invoke(int button, int state, int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int button, int state, int x, int y)
			{
				f_(button, state, x, y);
			}
		};

		static boost::shared_ptr<functor_base> tablet_button_functor_;

		inline void dummyTabletButtonFunc(int button, int state, int x, int y)
		{
			tablet_button_functor_->invoke(button, state, x, y);
		}

	} // namespace tablet_button_detail

	template <class F>
	inline void TabletButtonFunc(F f)
	{
		using namespace tablet_button_detail;

		tablet_button_functor_.reset(new functor<F>(f));
		::glutTabletButtonFunc(dummyTabletButtonFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void MenuStatusFunc(F f);                            //
	//                                                      //
	// glutMenuStatusFuncのテンプレート版                   //
	//   fはvoid f(int status, int x, int y)の              //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace menu_status_detail {

		class functor_base {
		public:
			virtual void invoke(int status, int x, int y) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int status, int x, int y)
			{
				f_(status, x, y);
			}
		};

		static boost::shared_ptr<functor_base> menu_status_functor_;

		inline void dummyMenuStatusFunc(int status, int x, int y)
		{
			menu_status_functor_->invoke(status, x, y);
		}

	} // namespace menu_status_detail

	template <class F>
	inline void MenuStatusFunc(F f)
	{
		using namespace menu_status_detail;

		menu_status_functor_.reset(new functor<F>(f));
		::glutMenuStatusFunc(dummyMenuStatusFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void MenuStateFunc(F f);                             //
	//                                                      //
	// glutMenuStateFuncのテンプレート版                    //
	//   fはvoid f(int status)の                            //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace menu_state_detail {

		class functor_base {
		public:
			virtual void invoke(int status) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int status)
			{
				f_(status);
			}
		};

		static boost::shared_ptr<functor_base> menu_state_functor_;

		inline void dummyMenuStateFunc(int status)
		{
			menu_state_functor_->invoke(status);
		}

	} // namespace menu_state_detail

	template <class F>
	inline void MenuStateFunc(F f)
	{
		using namespace menu_state_detail;

		menu_state_functor_.reset(new functor<F>(f));
		::glutMenuStateFunc(dummyMenuStateFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void IdleFunc(F f);                                  //
	//                                                      //
	// glutIdleFuncのテンプレート版                         //
	//   fはvoid f()の                                      //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace idle_detail {

		class functor_base {
		public:
			virtual void invoke() = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke()
			{
				f_();
			}
		};

		static boost::shared_ptr<functor_base> idle_functor_;

		inline void dummyIdleFunc()
		{
			idle_functor_->invoke();
		}

	} // namespace idle_detail

	template <class F>
	inline void IdleFunc(F f)
	{
		using namespace idle_detail;

		idle_functor_.reset(new functor<F>(f));
		::glutIdleFunc(dummyIdleFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void TimerFunc(unsigned int msecs, F f, int value);  //
	//                                                      //
	// glutTimerFuncのテンプレート版                        //
	//   fはvoid f(int value)の                             //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace timer_detail {

		class functor_base {
		public:
			virtual void invoke(int value) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(int value)
			{
				f_(value);
			}
		};

		static boost::shared_ptr<functor_base> timer_functor_;

		inline void dummyTimerFunc(int value)
		{
			timer_functor_->invoke(value);
		}

	} // namespace timer_detail

	template <class F>
	inline void TimerFunc(unsigned int msecs, F f, int value)
	{
		using namespace timer_detail;

		timer_functor_.reset(new functor<F>(f));
		::glutTimerFunc(msecs, dummyTimerFunc, value);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void WindowStatusFunc(F f);                          //
	//                                                      //
	// glutWindowStatusFuncのテンプレート版                 //
	//   fはvoid f(int state)の                             //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace state_detail {

		static boost::shared_ptr<functor_base> window_status_functor_;

		inline void dummyWindowStatusFunc(int state)
		{
			window_status_functor_->invoke(state);
		}

	} // namespace state_detail

	template <class F>
	inline void WindowStatusFunc(F f)
	{
		using namespace state_detail;

		window_status_functor_.reset(new functor<F>(f));
		::glutWindowStatusFunc(dummyWindowStatusFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void KeyboardUpFunc(F f);                            //
	//                                                      //
	// glutKeyboardUpFuncのテンプレート版                   //
	//   fはvoid f(unsigned char key, int x, int y)の       //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace keyboard_detail {

		static boost::shared_ptr<functor_base> keyboard_up_functor_;

		inline void dummyKeyboardUpFunc(unsigned char key, int x, int y)
		{
			keyboard_up_functor_->invoke(key, x, y);
		}

	} // namespace keyboard_detail

	template <class F>
	inline void KeyboardUpFunc(F f)
	{
		using namespace keyboard_detail;

		keyboard_up_functor_.reset(new functor<F>(f));
		::glutKeyboardUpFunc(dummyKeyboardUpFunc);
	}


	//------------------------------------------------------//
	// template <class F>                                   //
	// void SpecialUpFunc(F f);                             //
	//                                                      //
	// glutSpecialUpFuncのテンプレート版                    //
	//   fはvoid f(int key, int x, int y)の                 //
	//   シグニチャを持つ関数(オブジェクト)                 //
	//------------------------------------------------------//
	namespace special_detail {

		static boost::shared_ptr<functor_base> special_up_functor_;

		inline void dummySpecialUpFunc(int key, int x, int y)
		{
			special_up_functor_->invoke(key, x, y);
		}

	} // namespace special_detail

	template <class F>
	inline void SpecialUpFunc(F f)
	{
		using namespace special_detail;

		special_up_functor_.reset(new functor<F>(f));
		::glutSpecialUpFunc(dummySpecialUpFunc);
	}


	//--------------------------------------------------------------//
	// template <class F>                                           //
	// void JoystickFunc(F f, int pollInterval);                    //
	//                                                              //
	// glutJoystickFuncのテンプレート版                             //
	//   fはvoid f(unsigned int buttonMask, int x, int y, int z)の  //
	//   シグニチャを持つ関数(オブジェクト)                         //
	//--------------------------------------------------------------//
	namespace joystick_detail {

		class functor_base {
		public:
			virtual void invoke(unsigned int buttonMask, int x, int y, int z) = 0;
		};

		template <class F>
		class functor : public functor_base {
			F f_;
		public:
			functor(F f)
				: f_(f) {}

			void invoke(unsigned int buttonMask, int x, int y, int z)
			{
				f_(buttonMask, x, y, z);
			}
		};

		static boost::shared_ptr<functor_base> joystick_functor_;

		inline void dummyJoystickFunc(unsigned int buttonMask, int x, int y, int z)
		{
			joystick_functor_->invoke(buttonMask, x, y, z);
		}

	} // joystick_detail

	template <class F>
	inline void JoystickFunc(F f, int pollInterval)
	{
		using namespace joystick_detail;

		joystick_functor_.reset(new functor<F>(f));
		::glutJoystickFunc(dummyJoystickFunc, pollInterval);
	}


} // namespace glut

#endif // CPP_GLUT_INCLUDE