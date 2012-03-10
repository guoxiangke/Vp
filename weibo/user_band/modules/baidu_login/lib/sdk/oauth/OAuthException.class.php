<?php
/***************************************************************************
 *
 * Copyright (c)2009 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * OAuth Exception
 *
 * @package	oauth
 * @author	zhujt(zhujianting@baidu.com)
 * @version	$Revision: 1.0 $
 **/
class OAuthException extends Exception
{
    public function __construct($message, $code)
    {
    	parent::__construct($message, $code);
    }
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */