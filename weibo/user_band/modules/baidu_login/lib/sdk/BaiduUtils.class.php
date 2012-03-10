<?php
/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * Utils functions
 * 
 * @package	BaiduOpenAPI
 * @author	zhujt(zhujianting@baidu.com)
 * @version $Revision: 1.10 $
 **/
class BaiduUtils
{
	/**
	 * Generate a signature using the application secret key.
	 * 
	 * The only two entities that know your secret key are you and Baidu,
	 * according to the Terms of Service. Since nobody else can generate
	 * the signature, you can rely on it to verify that the information
	 * came from Baidu.
	 * 
	 * @param array $params	an array of all Baidu-sent parameters,
	 *                      NOT INCLUDING the signature itself
	 * @param string $secret	your app's secret key
	 * @return a hash to be checked against the signature provided by Baidu
	 */
	public static function generate_sig($params, $secret)
	{
		$str = '';
		ksort($params);
		//Note: make sure that the signature parameter is not already included in $params.
		foreach ($params as $k => $v) {
			$str .= "$k=$v";
		}
		$str .= $secret;
		return md5($str);
	}

	public static function no_magic_quotes($val)
	{
		if (get_magic_quotes_gpc()) {
			return stripslashes($val);
		} else {
			return $val;
		}
	}

	/**
	 * @brief urlencode a variable recursively, array keys and object property names will not be
	 * encoded, so you would better use ASCII to define the array key name or object property name.
	 *
	 * @param [in] mixed $var
	 * @return  mixed, with the same variable type
	 * @retval
	 * @see
	 * @note
	 * @author zhujt
	 * @date 2009/06/01 14:33:21
	**/
	public static function urlencode_recursive($var)
	{
		if (is_array($var)) {
			return array_map(array('BaiduUtils', 'urlencode_recursive'), $var);
		} elseif (is_object($var)) {
			$rvar = null;
			foreach ($var as $key => $val) {
				$rvar->{$key} = self::urlencode_recursive($val);
			}
			return $rvar;
		} elseif (is_string($var)) {
			return urlencode($var);
		} else {
			return $var;
		}
	}

	/**
	 * @brief urldecode a variable recursively, array keys and object property names will not be
	 * decoded, so you would better use ASCII to define the array key name or object property name.
	 *
	 * @param [in] mixed $var
	 * @return  mixed, with the same variable type
	**/
	public static function urldecode_recursive($var)
	{
		if (is_array($var)) {
			return array_map(array('BaiduUtils', 'urldecode_recursive'), $var);
		} elseif (is_object($var)) {
			$rvar = null;
			foreach ($var as $key => $val) {
				$rvar->{$key} = self::urldecode_recursive($val);
			}
			return $rvar;
		} elseif (is_string($var)) {
			return urldecode($var);
		} else {
			return $var;
		}
	}

	/**
	 * @brief base64_encode a variable recursively, array keys and object property names will not be
	 * encoded, so you would better use ASCII to define the array key name or object property name.
	 *
	 * @param [in] mixed $var
	 * @return  mixed, with the same variable type
	**/
	public static function base64_encode_recursive($var)
	{
		if (is_array($var)) {
			return array_map(array('BaiduUtils', 'base64_encode_recursive'), $var);
		} elseif (is_object($var)) {
			$rvar = null;
			foreach ($var as $key => $val) {
				$rvar->{$key} = self::base64_encode_recursive($val);
			}
			return $rvar;
		} elseif (is_string($var)) {
			return base64_encode($var);
		} else {
			return $var;
		}
	}

	/**
	 * @brief base64_decode a variable recursively, array keys and object property names will not be
	 * decoded, so you would better use ASCII to define the array key name or object property name.
	 *
	 * @param [in] mixed $var
	 * @return  mixed, with the same variable type
	**/
	public static function base64_decode_recursive($var)
	{
		if (is_array($var)) {
			return array_map(array('BaiduUtils', 'base64_decode_recursive'), $var);
		} elseif (is_object($var)) {
			$rvar = null;
			foreach ($var as $key => $val) {
				$rvar->{$key} = self::base64_decode_recursive($val);
			}
			return $rvar;
		} elseif (is_string($var)) {
			return base64_decode($var);
		} else {
			return $var;
		}
	}

	/**
	 * @brief Encode the GBK format var into json format.
	 *
	 * @param [in] mixed $var	The value being encoded. Can be any type except a resource.
	 * @return json format string.
	 * @note The standard json_encode & json_decode needs all strings be in ASCII or UTF-8 format,
	 * but most of the time, we use GBK format strings and the standard ones will not work properly,
	 * by base64_encoded the strings we can change them to ASCII format and let the json_encode &
	 * json_decode functions work.
	**/
	public static function json_encode($var)
	{
		return json_encode(self::base64_encode_recursive($var));
	}

	/**
	 * @brief Decode the GBK format var from json format.
	 *
	 * @param [in] string $json	json formated string
	 * @param [in] bool $assoc	When TRUE, returned objects will be converted into associative arrays.
	 * @return mixed, associated array with values be urldecoded
	 * @note The standard json_encode & json_decode needs all strings be in ASCII or UTF-8 format,
	 * but most of the time, we use GBK format strings and the standard ones will not work properly,
	 * by base64_encoded the strings we can change them to ASCII format and let the json_encode &
	 * json_decode functions work.
	**/
	public static function json_decode($json, $assoc = false)
	{
		return self::base64_decode_recursive(json_decode($json, $assoc));
	}

	/**
	 * @brief Convert string or array to requested character encoding
	 *
	 * @param mix $var	variable to be converted
	 * @param string $in_charset	The input charset.
	 * @param string $out_charset	The output charset
	 * @return mix	The array with all of the values in it noslashed
	 * @retval The array with all of the values in it noslashed
	 * @see http://cn2.php.net/manual/en/function.iconv.php
	 * @note
	 * @author zhujt
	 * @date 2009/03/16 12:17:09
	**/
	public static function iconv_recursive($var, $in_charset = 'UTF-8', $out_charset = 'GBK')
	{
		if (is_array($var)) {
			$rvar = array();
			foreach ($var as $key => $val) {
				$rvar[$key] = self::iconv_recursive($val, $in_charset, $out_charset);
			}
			return $rvar;
		} elseif (is_object($var)) {
			$rvar = null;
			foreach ($var as $key => $val) {
				$rvar->{$key} = self::iconv_recursive($val, $in_charset, $out_charset);
			}
			return $rvar;
		} elseif (is_string($var)) {
			return iconv($in_charset, $out_charset, $var);
		} else {
			return $var;
		}
	}
}


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>
