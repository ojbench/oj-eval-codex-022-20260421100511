#!/usr/bin/env python3
import requests
import argparse
import os
import json
from datetime import datetime


def main():
    parser = argparse.ArgumentParser(description='Submit raw code to ACMOJ')
    parser.add_argument('--token', default=os.environ.get('ACMOJ_TOKEN'))
    parser.add_argument('--problem-id', type=int, required=True)
    parser.add_argument('--file', type=str, required=True)
    parser.add_argument('--language', type=str, default='cpp')
    args = parser.parse_args()

    if not args.token:
        print('Error: Access token not provided. Use --token or set ACMOJ_TOKEN')
        return

    with open(args.file, 'r') as f:
        code = f.read()

    api_base = 'https://acm.sjtu.edu.cn/OnlineJudge/api/v1'
    headers = {
        'Authorization': f'Bearer {args.token}',
        'Content-Type': 'application/x-www-form-urlencoded',
        'User-Agent': 'ACMOJ-Python-Client/2.2'
    }
    data = {'language': args.language, 'code': code}

    r = requests.post(f{api_base}/problem/{args.problem_id}/submit, headers=headers, data=data, timeout=10, proxies={'https': None, 'http': None})
    r.raise_for_status()
    res = r.json()
    if 'id' in res:
        try:
            ts = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            with open('/workspace/submission_ids.log', 'a') as lf:
                lf.write(json.dumps({'timestamp': ts, 'submission_id': res['id']}) + '\n')
            print('Submission ID {} saved to {}'.format(res['id'], '/workspace/submission_ids.log'))
        except Exception as e:
            print('Warning: Failed to save submission ID: {}'.format(e))
    print(json.dumps(res))


if __name__ == '__main__':
    main()
