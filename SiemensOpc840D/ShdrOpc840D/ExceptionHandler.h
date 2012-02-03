//
// ExceptionHandler.h
//

#pragma once


template 
class AutoRegister : public RegistrationRecord
{
public:
	/** Auto-register an exception record for 'RegistrationRecord' */
	AutoRegister()
		: RegistrationRecord()
	{
		registerHandler( this );
	}

	/** Unregister and destroy an exception record */
	~AutoRegister()
	{
		unregisterHandler( this );
	}
};

class ExceptionHelperBase : public _EXCEPTION_REGISTRATION_RECORD
{
public:
	/** Construct helper object */
	ExceptionHelperBase();

	/** Make safe to extend */
	virtual ~ExceptionHelperBase() {}

	/** Allow subclass to hook exception */
	virtual void onException( EXCEPTION_RECORD *pException, CONTEXT *pContext ) = 0;

private:
	// Disable copy and assign
	ExceptionHelperBase( ExceptionHelperBase const & );
	ExceptionHelperBase& operator=( ExceptionHelperBase const & );

	// The one and only exception handler function
	static fnExceptionHandler exceptionHandler;
};

DWORD ExceptionHelperBase::exceptionHandler( EXCEPTION_RECORD *pException, struct _EXCEPTION_REGISTRATION_RECORD *pRegistrationRecord, CONTEXT *pContext )
{
	ExceptionHelperBase &self = static_cast( *pRegistrationRecord );
	self.onException( pException, pContext );
	return ExceptionContinueSearch;
}

class ExceptionHelperImpl1 : public ExceptionHelperBase
{
	/** Print the address of the exception records */
	virtual void onException( EXCEPTION_RECORD *pException, CONTEXT *pContext )
	{
		printf( "pException: %p (code: %p, flags: %x), pContext: %p\n", pException, pException->ExceptionCode, pException->ExceptionFlags, pContext );
	}
};

typedef AutoRegister< ExceptionHelperImpl1 > ExceptionHelper1;

class ExceptionStackTraceImpl : public ExceptionHelperBase
{
public:
	ExceptionStackTraceImpl() : pSavedContext(0) {}

	/** Use the saved pointer to print the stack trace */
	void printStackTrace( std::ostream & os ) const
	{
		if ( pSavedContext != 0 )
			SimpleSymbolEngine::instance().StackTrace( pSavedContext, os );
	}

private:
	/** Capture the thread context when the initial exception occurred */
	virtual void onException( EXCEPTION_RECORD *pException, CONTEXT *pContext )
	{
		if ( ( pException->ExceptionFlags & EXCEPTION_UNWINDING ) == 0 )
		{
			pSavedContext = pContext;
		}
	}

	PCONTEXT pSavedContext; // context record from the last exception
};

typedef AutoRegister< ExceptionStackTraceImpl > ExceptionStackTrace;