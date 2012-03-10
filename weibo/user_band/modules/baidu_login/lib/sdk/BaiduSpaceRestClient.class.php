<?php
/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * Rest Client for Baidu Space
 * 
 * @package	BaiduOpenAPI
 * @author	zhujt(zhujianting@baidu.com)
 * @version $Revision: 1.20 $ 
 **/
class BaiduSpaceRestClient extends BaiduRestClient
{
	/**
	 * Constructor
	 * @param BaiduRestClient $api_client
	 */
	public function __construct(BaiduRestClient $api_client)
	{ 
		parent::__construct($api_client->api_key, $api_client->secret, $api_client->session_key);
		
		$this->using_session_secret = $api_client->using_session_secret;
		
		$this->user			= $api_client->user;
		$this->friends_list	= $api_client->friends_list;
		$this->is_user		= $api_client->is_user;
		
		$this->http_method	= $api_client->get_http_method();
		$this->format		= $api_client->get_format();
		$this->final_encode	= $api_client->get_final_encode();
	}

	/**
	 * Return the openapi version of current baidu product
	 * 
	 * Extended classes may rewrite this interface as needed
	 *
	 * @return string
	**/
	public function openapi_version()
	{
		return '1.0';
	}

	/**
	 * Classes extend from this class should override it to provide a corrent address
	 * 
	 * @return string
	 */
	public function get_restserver_url()
	{
		return BD_SPACE_OPENAPI_DOMAIN . '/restserver/space';
	}

	//api opened by space product
	
	/**
	 * 公共授权（免授权）的API接口的demo示例
	 * @param mix $param1
	 * @param mix $param2
	 * @return array
	 */
	public function publicAuthMethodDemo($param1, $param2)
	{
		$params = array('param1' => $param1, 'param2' => $param2);
		return $this->call_method('baidu.space.publicAuthMethodDemo', $params);
	}
	
	/**
	 * 应用授权的API接口的demo示例
	 * @param mix $param1
	 * @param mix $param2
	 * @param mix $param3
	 * @return array
	 */
	public function appAuthMethodDemo($param1, $param2, $param3)
	{
		$params = array('param1' => $param1, 'param2' => $param2, 'param3' => $param3);
		return $this->call_method('baidu.space.appAuthMethodDemo', $params);
	}
	
	/**
	 * Get the specified user's space info
	 * if both $spaceurl and $uid are null, current logged in user's space info will be
	 * returned, if both are specified, space info related to $uname will be returned
	 * 
	 * @param string $spaceurl
	 * @param string $uname
	 * @return array
	 * <code>
	 * array(  'username' => string,
	 * 			'userid' => int,
	 * 			'space_url' => string,
	 * 			'space_name' => string,
	 * 			'space_desc' => string,
	 * 			'ground_url' => string,
	 * 			'space_id' => uint,
	 * 			'create_time' => uint,
	 * 			'modify_time' => uint,
	 * 			'css_id' => string,
	 * 			'portrait' => string)
	 * </code>
	**/
	public function space_getInfo($spaceurl, $uname = null)
	{
		$params = array('spaceurl' => $spaceurl, 'uname' => $uname);
		return $this->call_method('baidu.space.space.getInfo', $params);
	}

	/**
	 * Get the space info for users who are space user specified in $uids
	 *
	 * @param string $uids	the specified users, seperated by ',', if null,
	 * 						current logged in user is requested
	 * @return array
	 * <code>
	 * array(0 => object('uid' => uint,
	 * 					'space_id' => uint,
	 * 					'username' => string,
	 *					'space_url' => string),
	 *		...
	 * )
	 * </code>
	**/
	public function space_getSpaceUserInfos($uids)
	{
		return $this->call_method('baidu.space.space.getSpaceUserInfos', array('uids' => $uids));
	}
}

// Supporting methods and values------


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>
