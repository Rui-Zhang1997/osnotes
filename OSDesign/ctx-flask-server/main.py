import pymysql as pysql
import requests as req
import json

BASE_URL = 'https://newsapi.org/v2/'
HEADLINES_API = BASE_URL + 'top-headlines'
# KEYS FOR NEW SOURCES
# ALL OTHERS ARE SCRAPED
WAPO_KEY = '05fa6d5baabb4318bd9351a4ae037cf3'
CNN_KEY = 'ykj3u7wrqtyz7mz5try9akrj'
NYT_KEY = 'f1fddb5d7fd4bd083599f154bdaae49'

connection = pysql.connect(
    host='localhost',
    user='horimori',
    password='ts121389',
    db='ctx',
    charset='utf8mb4',
    cursorclass=pysql.cursors.DictCursor)

def get_headlines_for_country(country = 'us'):
    return req.get(HEADLINES_API, params = {'country': country, 'apiKey': WAPO_KEY})

def create_sql_stmt(table, kvpairs):
    pass

def insert_to_db(articles):
    print("ARTICLES: ", len(articles))
    sql_stmt_am = 'INSERT INTO article_meta (author, title, description, url, urlToImage, publishedAt, sid) VALUES (%s, %s, %s, %s, %s, %s, %s)'
    sql_stmt_src = 'INSERT INTO source (sid, name) VALUES (%s, %s) ON DUPLICATE KEY UPDATE name=name'
    source_tuples = set()
    article_tuples = []
    for a in articles:
        src = a['source']
        if not src['id']:
            src['id'] = src['name']
        source_tuples.add((src['id'], src['name']))
        article_tuples.append((a['author'], a['title'], a['description'], a['url'], a['urlToImage'], a['publishedAt'], src['id']))
    with connection.cursor() as cursor:
        for src in source_tuples:
            cursor.execute(sql_stmt_src, src)
        for a in article_tuples:
            cursor.execute(sql_stmt_am, a)
    connection.commit()

def
r = get_headlines_for_country()
insert_to_db(r.json()['articles'])
